#include <dvifile.h>
#include <Ancillary/ByteBuffer.h>
#include <stdio.h>

using namespace dviview;

int main(int argc, char **argv) {
  if(argc < 2)
    return 1;
  char *filename = argv[1];
  ByteBuffer buf;
  if (ByteBuffer::initFromFile(filename, buf)) {
    fprintf(stderr, "Unable to read file!\n");
    return 1;
  }
  DVIFileParser parser;
  if (parser.parse(buf)) {
    fprintf(stderr, "Unable to parse DVI file!\n");
    return 1;
  }
  
  std::vector<DVIOperation> ops = parser.operations();
  for (std::vector<DVIOperation>::iterator iter = ops.begin(); iter != ops.end(); iter++) {
    DVIOperation op = *iter;
    printf("%s", dviOPTable[op.getOpcode()].name);
    for (unsigned operIndex = 0; operIndex < op.getOperands().size(); operIndex++) {
      DVIOperation::Operand oper = op.getOperands()[operIndex];
      DVIOperandType type = dviOPTable[op.getOpcode()].operands[operIndex];
      switch(type) {
        case Int1:
        case Int2:
        case Int3:
        case Int4:
          printf(" %d", oper.integer);
          break;
        case BString1:
        case BString2:
        case BString3:
        case BString4:
          printf(" \"");
          for (unsigned stringIndex = 0; stringIndex < oper.string->getSize(); stringIndex++) {
            uint8_t byte = (*oper.string)[stringIndex];
            printf("%02X", byte);
          }
          printf("\"");
          break;
        case SumString:
        case String1:
          printf(" \"");
          for (unsigned stringIndex = 0; stringIndex < oper.string->getSize(); stringIndex++) {
            uint8_t chr = (*oper.string)[stringIndex];
            printf("%c", chr);
          }
          printf("\"");
          break;
        default:
          printf(" <Unknown operand type>");
      }
      
    }
    printf("\n");
  }
  return 0;
}
