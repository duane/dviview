#ifndef __INCLUDE_PKFILE_H__
#define __INCLUDE_PKFILE_H__

#include <stdint.h>
#include <vector>

#include <Ancillary/ByteString.h>
#include <Ancillary/ByteBuffer.h>
#include <Ancillary/SmallVector.h>

namespace dviview {

enum {
  pk_xxx1 = 240,
  pk_xxx2 = 241,
  pk_xxx3 = 242,
  pk_xxx4 = 243,
  pk_yyy = 244,
  pk_post = 245,
  pk_no_op = 246,
  pk_pre = 247,
};

enum PKOperandType {
  PKBString,
  PKString,
  PKInt,
};

struct PKOperand {
  PKOperandType type;
  union {
    int32_t integer;
    ByteString *string;
  };
};

struct PKOperation {
  uint8_t opcodeFlag;
  uint64_t offset;
  SmallVector<PKOperand, 11> operands;
};

class PKFileParser {
  private:
    DISALLOW_COPY_AND_ASSIGN(PKFileParser);
    std::vector<PKOperation> operations;
  public:
    PKFileParser() : operations(std::vector<PKOperation>()) {}
	std::vector<PKOperation> getOperations() {return operations;}
	
    int parseBuffer(ByteBuffer &buffer);
};

class PKGlyph {

};

class PKTypeFace {
  
};

}  // namespace dviview

#endif  // __INCLUDE_PKFILE_H__
