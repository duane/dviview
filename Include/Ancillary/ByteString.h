#ifndef __INCLUDE_ANCILLARY_BYTESTRING_H__
#define __INCLUDE_ANCILLARY_BYTESTRING_H__

#include <assert.h>
#include <string.h>

#include <Ancillary/CXXSupport.h>

namespace dviview {

/*
 *  ByteString is a datatype for byte strings for <=256 bytes.
 *  It is designed for the string to be stored in the object itself,
 *  rather than storing a reference to it. This means that references
 *  can be passed around on the stack, and the data can be accessed
 *  with just one indirection.
 */
class ByteString {
  private:
    // Copy/assign allowed.

    size_t size;    
    uint8_t *string;
  public:
    ByteString() : size(0), string(NULL) {}
    ByteString(uint8_t *buf, size_t bufSize) : size(bufSize) {
      string = (uint8_t*)malloc(bufSize);
      memcpy(string, buf, bufSize);
    }

    ~ByteString() {
      free(string);
    }
  
    uint8_t *begin() {
      return string;
    }

    uint8_t *end() {
      return string + size;
    }

    uint8_t operator[](size_t index) {
      assert(index < size);
      return string[index];
    }

    size_t getSize() {
      return size;
    }
};
}  // namespace dviview

#endif  // __INCLUDE_ANCILLARY_BYTESTRING_H__
