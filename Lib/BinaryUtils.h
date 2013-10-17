#ifndef __LIB_BINARYUTILS_H__
#define __LIB_BINARYUTILS_H__

#include <stdint.h>
#include <Ancillary/ByteBuffer.h>
#include <Ancillary/ByteString.h>
#include <Ancillary/endian.h>

namespace dviview {

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

static int readUIntOfSize(unsigned size, uint64_t &index, ByteBuffer &buf, uint32_t &result) {
  switch(size) {
    case 1:
      uint8_t int1;  
      if (buf.getByte(index, &int1))
        return -1;
      result = int1;
      index += 1;
      return 0;
    case 2:
      uint16_t int2;
      if (buf.getBytes(index, 2, (uint8_t*)&int2))
        return -1;
      result = BigToHost16(int2);
      index += 2;
      return 0;
    case 3:
      uint8_t int3Buf[3];
      if (buf.getBytes(index, 3, (uint8_t*)&int3Buf))
        return -1;
      result = int3Buf[0] << 16 | int3Buf[1] << 8 | int3Buf[2];
      index += 3;
      return 0;
    case 4:
      uint32_t int4;
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

} // namespace dviview

#endif  // __LIB_BINARYUTILS_H__
