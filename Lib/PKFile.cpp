#include <PKFile.h>

#include "BinaryUtils.h"

using namespace dviview;

static inline PKOperand createPKOperandInt(int32_t i) {
  PKOperand op;
  op.type = PKInt;
  op.integer = i;
  return op;
}

static inline PKOperand createPKOperandString(PKOperandType type, ByteString *string) {
  PKOperand op;
  op.type = type;
  op.string = string;
  return op;
}

int readXXX(unsigned size, uint64_t &index, ByteBuffer &buffer, PKOperation &result) {
  assert(size > 0 && size <= 4 && "Invalid xxx size value");

  ByteString a;	
  PKOperand operand;
  operand.type = PKBString;
  if (readStringOfSize(size, index, buffer, &operand.string))
    return -1;
  result.operands.push_back(operand);

  return 0;
}

int readYYY(uint64_t &index, ByteBuffer &buffer, PKOperation &result) {
  PKOperand operand;
  operand.type = PKInt;
  if (readIntOfSize(4, index, buffer, operand.integer))
    return -1;
  result.operands.push_back(operand);
  return 0;
}

int readPre(uint64_t &index, ByteBuffer &buffer, PKOperation &result) {
  ByteString *x;
  int32_t i, ds, cs, hppp, vppp;

  if (readIntOfSize(1, index, buffer, i)) {
    return -1;
  }
  
  if (readStringOfSize(1, index, buffer, &x))
    return -1;

  if (readIntOfSize(4, index, buffer, ds))
    return -1;

  if (readIntOfSize(4, index, buffer, cs))
    return -1;

  if (readIntOfSize(4, index, buffer, hppp))
    return -1;

  if (readIntOfSize(4, index, buffer, vppp))
    return -1;
  
  result.operands.push_back(createPKOperandInt(i));
  result.operands.push_back(createPKOperandString(PKString, x));
  result.operands.push_back(createPKOperandInt(ds));
  result.operands.push_back(createPKOperandInt(cs));
  result.operands.push_back(createPKOperandInt(hppp));
  result.operands.push_back(createPKOperandInt(vppp));
  return 0;
}

int readChar(uint64_t &index, ByteBuffer &buffer, PKOperation &result) {
  uint8_t flag = result.opcodeFlag;

  uint8_t lsb3 = flag & 0x7;
  
  enum {
    SHORT,
    EXTENDED,
    LONG
  };
  unsigned type, intWidth;
  if (lsb3 <= 3) {
    type = SHORT;
    intWidth = 1;
  } else if (lsb3 <= 6) {
    type = EXTENDED;
    intWidth = 2;
  } else {
    type = LONG;
    intWidth = 4;
  }


  uint32_t pl, cc, tfm, w, h;
  int32_t dm, dx, dy, hoff, voff;
  
  result.operands.push_back(createPKOperandInt(flag));

  if (readUIntOfSize(intWidth, index, buffer, pl))
    return -1;
  pl = pl + ((flag & 0x3) << 8);
  result.operands.push_back(createPKOperandInt(pl));


  if (readUIntOfSize((type == LONG) ? 4 : 1, index, buffer, cc))
    return -1;
  result.operands.push_back(createPKOperandInt(cc));

  if (readUIntOfSize((type == LONG) ? 4 : 3, index, buffer, tfm))
    return -1;
  result.operands.push_back(createPKOperandInt(tfm));
  
  if (type != LONG) {
    if (readIntOfSize(intWidth, index, buffer, dm))
      return -1;
    result.operands.push_back(createPKOperandInt(dm));
  } else {
    if (readIntOfSize(intWidth, index, buffer, dx))
      return -1;
    result.operands.push_back(createPKOperandInt(dx));
    
    if (readIntOfSize(intWidth, index, buffer, dy))
      return -1;
    result.operands.push_back(createPKOperandInt(dy));
  }
  
  if (readUIntOfSize(intWidth, index, buffer, w))
    return -1;
  result.operands.push_back(createPKOperandInt(w));
  
  if (readUIntOfSize(intWidth, index, buffer, h))
    return -1;
  result.operands.push_back(createPKOperandInt(h));
  
  if (readIntOfSize(intWidth, index, buffer, hoff))
    return -1;
  result.operands.push_back(createPKOperandInt(hoff));
  
  if (readIntOfSize(intWidth, index, buffer, voff))
    return -1;
  result.operands.push_back(createPKOperandInt(voff));
  
  unsigned rasterLen;
  // now read the rest of the packet length into a byte string.
  if (type == SHORT)
    rasterLen = pl - 8;
  else if (type == EXTENDED)
    rasterLen = pl - 13;
  else
    rasterLen = pl - 27;
  
  uint8_t *rasterData = (uint8_t*)malloc(rasterLen);
  if (buffer.getBytes(index, rasterLen, rasterData)) {
    fprintf(stderr, "Error reading raster length for length: %u.\n", rasterLen);
    free(rasterData);
    return -1;
  }
  index += rasterLen;
  ByteString *str = new ByteString(rasterData, rasterLen);
  free(rasterData);
  result.operands.push_back(createPKOperandString(PKBString, str));
  // Success!
  return 0;
}

int PKFileParser::parseBuffer(ByteBuffer &buffer) {
  uint64_t index = 0;
  while (index < buffer.size()) {
    PKOperation result;
    result.offset = index;
    if (buffer.getByte(index, &result.opcodeFlag))
      return -1;
    index += 1;
    int status;
    switch (result.opcodeFlag) {
      case pk_xxx1:
        status = readXXX(1, index, buffer, result);
        break;
      case pk_xxx2:
        status = readXXX(2, index, buffer, result);
        break;
      case pk_xxx3:
        status = readXXX(3, index, buffer, result);
        break;
      case pk_xxx4:
        status = readXXX(4, index, buffer, result);
        break;
      case pk_yyy:
        status = readYYY(index, buffer, result);
        break;
      case pk_post:
      case pk_no_op:
        status = 0;
        break;
      case pk_pre:
        status = readPre(index, buffer, result);
        break;
      case 248:
      case 249:
      case 250:
      case 251:
      case 252:
      case 253:
      case 254:
      case 255: {
        fprintf(stderr, "Undefined operation!\n");
        return -1;
      }
      default:
        status = readChar(index, buffer, result);
        if (status)
          fprintf(stderr, "Error reading char.\n");
        break;
    }

    if (status) {
      fprintf(stderr, "Error reading operation.\n");
      return -1;
    }
    operations.push_back(result);
  }
  return 0;
}
