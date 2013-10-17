#include <PKFile.h>
#include <stdio.h>

using namespace dviview;

void printPKOperation(PKOperation op) {
  ByteString *string;
  printf("%2X@%llu:", op.opcodeFlag, op.offset);
  for (SmallVectorTemplateCommon<PKOperand>::iterator iter = op.operands.begin(); iter != op.operands.end(); iter++) {
    printf(" ");
    PKOperand op = *iter;
    switch(op.type) {
      case PKBString:
        string = op.string;
        printf("\"");
        for (uint8_t *byte_p = string->begin(); byte_p != string->end(); byte_p++) {
          printf("%02X", *byte_p);
        }
        printf("\"");
        break;
      case PKString:
        string = op.string;
        printf("\"");
        for (uint8_t *byte_p = string->begin(); byte_p != string->end(); byte_p++) {
          printf("%c", *byte_p);
        }
        printf("\"");
        break;
      case PKInt:
        printf("%d", op.integer);
        break;
    }
  }
  printf("\n");
}

void printPKFile(PKFileParser &parser) {
  std::vector<PKOperation> operations = parser.getOperations();
  for (std::vector<PKOperation>::iterator iter = operations.begin(); iter != operations.end(); iter++) {
    PKOperation op = *iter;
    printPKOperation(op);
  }
}

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "bad arguments.\n");
    return 1;
  }
  char *filename = argv[1];
  
  ByteBuffer buf;
  if (ByteBuffer::initFromFile(filename, buf)) {
    fprintf(stderr, "Error reading from file.\n");
    return 1;
  }
    

  PKFileParser parser;
  if (parser.parseBuffer(buf)) {
    fprintf(stderr, "Error parsing file.\n");
    return 1;
  }

  printPKFile(parser);

  return 0;
}
