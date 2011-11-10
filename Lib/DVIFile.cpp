#include <DVIFile.h>
#include <Ancillary/endian.h>

using namespace dviview;

static int readIntOfSize(unsigned size, uint64_t &index, ByteBuffer &buf, int32_t &result) {
  switch(size) {
    case 1:
      int8_t int1;  
      if (buf.getByte(index, (uint8_t*)&int1))
        return -1;
      result = int1;
      index += 1;
      return 0;
    case 2:
      int16_t int2;
      if (buf.getBytes(index, 2, (uint8_t*)&int2))
        return -1;
      result = BigToHost16(int2);
      index += 2;
      return 0;
    case 3:
      int8_t int3Buf[3];
      if (buf.getBytes(index, 3, (uint8_t*)&int3Buf))
        return -1;
      result = int3Buf[0] << 16 | int3Buf[1] << 8 | int3Buf[2];
      index += 3;
      return 0;
    case 4:
      int32_t int4;
      if (buf.getBytes(index, 4, (uint8_t*)&int4))
        return -1;
      result = BigToHost32(int4);
      index += 4;
      return 0;
    default:
      return -1;
  }
}

static int readStringOfSize(unsigned size, uint64_t &index, ByteBuffer &buffer, ByteString **result) {
  int32_t len;
  if (readIntOfSize(size, index, buffer, len))
    return -1;
  uint8_t *bytes = (uint8_t*)malloc(len);
  if (buffer.getBytes(index, len, bytes))
    return -1;
  *result = new ByteString(bytes, len);
  index += len;
  return 0;
}

int DVIFileParser::readOperand(ByteBuffer &buffer, DVIOperation::Operand &operand, DVIOperandType type) {
  ByteString *result;
  switch(type) {
    case Int1:
      if (readIntOfSize(1, index, buffer, operand.integer))
        return -1;
      return 0;
    case Int2:
      if (readIntOfSize(2, index, buffer, operand.integer))
        return -1;
      return 0; 
    case Int3:
      if (readIntOfSize(3, index, buffer, operand.integer))
        return -1;
      return 0;
    case Int4:
      if (readIntOfSize(4, index, buffer, operand.integer))
        return -1;
      return 0;
    case String1:
      if (readStringOfSize(1, index, buffer, &result))
        return -1;
      operand.string = result;
      return 0;
    case String2:
      if (readStringOfSize(2, index, buffer, &result))
        return -1;
      operand.string = result;
      return 0;
    case String3:
      if (readStringOfSize(3, index, buffer, &result))
        return -1;
      operand.string = result;
      return 0;
    case String4:
      if (readStringOfSize(4, index, buffer, &result))
        return -1;
      operand.string = result;
      return 0;
    case SumString:
      {
        uint8_t a, l;
        if (readIntOfSize(1, index, buffer, (int32_t&)a))
          return -1;
        if (readIntOfSize(1, index, buffer, (int32_t&)l))
          return -1;
        uint32_t len = (uint32_t)a + (uint32_t)l;
        uint8_t sumString[len];
        if (buffer.getBytes(index, len, sumString))
          return -1;
        index += len;
        operand.string = new ByteString(sumString, len);
        return 0;
      }
    default:
      return -1;
  }
}

int DVIFileParser::readOperation(ByteBuffer &buffer, DVIOperation &operation) {
  uint8_t opcode;
  SmallVector<DVIOperation::Operand, 10> opers;
  if (buffer.getByte(index, &opcode))
    return -1;
  index += 1;

  DVIOperationDefinition def = dviOPTable[opcode];
  unsigned i = 0;
  while (def.operands[i] != NullOperand) {
    DVIOperation::Operand operand;
    if (readOperand(buffer, operand, def.operands[i]))
      return -1;
    opers.push_back(operand);
    i++;
  }

  operation = DVIOperation(opcode, opers);
  return 0;
}

int DVIFileParser::parse(ByteBuffer &buf) {
  index = 0;
  while (index < buf.size()) {
    DVIOperation op;
    if (readOperation(buf, op))
      return -1;
    ops.push_back(op);
  }
  return 0;
}

