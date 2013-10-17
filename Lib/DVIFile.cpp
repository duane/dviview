#include <DVIFile.h>
#include <Ancillary/endian.h>

#include "BinaryUtils.h"

using namespace dviview;


int DVIFileParser::readOperand(ByteBuffer &buffer, DVIOperation::Operand &operand, DVIOperandType type) {
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
      if (readStringOfSize(1, index, buffer, &operand.string))
        return -1;
      return 0;
    case BString1:
      if (readStringOfSize(1, index, buffer, &operand.string))
        return -1;
      return 0;
    case BString2:
      if (readStringOfSize(2, index, buffer, &operand.string))
        return -1;
      return 0;
    case BString3:
      if (readStringOfSize(3, index, buffer, &operand.string))
        return -1;
      return 0;
    case BString4:
      if (readStringOfSize(4, index, buffer, &operand.string))
        return -1;
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
  for (unsigned i = 0; def.operands[i] != NullOperand; i++) {
    DVIOperation::Operand operand;
    if (readOperand(buffer, operand, def.operands[i]))
      return -1;
    opers.push_back(operand);
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

