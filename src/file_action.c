#include "file_action.h"

void file_rewriting(FILE *file_to_cmp, FILE *cmp_file) {
  unsigned char buffer[128];
  size_t numread;
  while (!feof(file_to_cmp)) {
    numread = fread(buffer, sizeof(unsigned char), 128, file_to_cmp);
    fwrite(buffer, sizeof(unsigned char), numread, cmp_file);
  }
}