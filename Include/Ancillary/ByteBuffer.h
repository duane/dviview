#ifndef __INCLUDE_ANCILLARY_BYTEBUFFER_H__
#define __INCLUDE_ANCILLARY_BYTEBUFFER_H__

#include <stddef.h>
#include <stdint.h>

#include <Ancillary/CXXSupport.h>

namespace dviview {

class ByteBuffer {
  private:
    DISALLOW_COPY_AND_ASSIGN(ByteBuffer);

    uint8_t *rawBuffer;
    uint64_t bufferSize;
  public:
    ByteBuffer() : rawBuffer(NULL), bufferSize(0) {}

    static int initFromFile(const char *path, ByteBuffer &result);
    int getByte(size_t offset, uint8_t *byte);
    int getBytes(size_t offset, size_t size, uint8_t *bytes);
    inline uint64_t size() {return bufferSize;}
};

}  // namespace dviview

#endif  // __INCLUDE_ANCILLARY_BYTEBUFFER_H__
