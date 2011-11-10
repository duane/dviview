#ifndef __INCLUDE_ANCILLARY_BINARYSTREAM_H__
#define __INCLUDE_ANCILLARY_BINARYSTREAM_H__

#include <Ancillary/CXXSupport.h>

namespace dviview {

class BinaryStream {
  public:
    virtual ~BinaryStream();
    
    virtual uint64_t size() const = 0;
    virtual int readByte(uint8_t *result) const = 0;
    virtual int readBytes(size_t size, uint8_t *result) const = 0;
    virtual int seek(size_t offset) const = 0;
    virtual size_t offset() const = 0;
};


}  // namespace dviview

#endif  // __INCLUDE_ANCILLARY_BINARYSTREAM_H__
