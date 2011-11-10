#include <Ancillary/ByteBuffer.h>

#include <stdio.h>
#include <stdlib.h>

#include <sys/stat.h>

using namespace dviview;

int ByteBuffer::initFromFile(const char *path, ByteBuffer &result) {
  FILE *file = fopen(path, "r");
  if (!file)
    return -1;

  int fildes = fileno(file);
  struct stat fileStat;
  if (fstat(fildes, &fileStat))
     return -1;
  size_t fileSize = fileStat.st_size;
  
  uint8_t *rawBuffer = (uint8_t*)malloc(fileSize);
  if (!rawBuffer)
    return -1;

  if (fread(rawBuffer, 1, fileSize, file) != fileSize) {
    free(rawBuffer);
    return -1;
  }

  if(fclose(file))
    return -1;

  result.rawBuffer = rawBuffer;
  result.bufferSize = fileSize;
  return 0;
}

int ByteBuffer::getByte(size_t offset, uint8_t *byte) {
  if (offset > bufferSize)
    return -1;
  *byte = rawBuffer[offset];
  return 0;
}

int ByteBuffer::getBytes(size_t offset, size_t size, uint8_t *bytes) {
  if (offset + size > bufferSize)
    return -1;

  for (size_t index = 0; index < size; index++) {
    size_t indexPrime = offset + index;
    if (getByte(indexPrime, &bytes[index]))
      return -1;
  }
  return 0;
}
