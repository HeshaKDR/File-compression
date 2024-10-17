#include "huffman.h"

#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
  int i = 3;
  int size = 0;
  //   int flag = 0;

  if (argc <= i) {
    printf("syntaxis error!\ncmptool syntaxis: cmptool -flag('c' - to "
           "compress, 'd' - to decompress) "
           "<file_to_compress.txt> "
           "<name_of_compressed_file.cmp>\n");
  } else {
    if (strcmp(argv[1], "-c") == 0) {
      FILE *existing_file = NULL;
      existing_file = fopen(argv[2], "rb");
      if (existing_file != NULL) {
        fseek(existing_file, 0, SEEK_END);
        size = ftell(existing_file);
        fseek(existing_file, 0, SEEK_SET);

        if (size == 0) {
          printf("file to compress is empty\nnothing to compress\n");
          fclose(existing_file);
        } else {
          FILE *cmp_file = NULL;
          cmp_file = fopen(argv[3], "wb");
          compress_file(existing_file, cmp_file);
          fseek(cmp_file, 0, SEEK_END);
          int cmp_file_size = ftell(cmp_file);
          printf("ORIGINAL FILE SIZE: %d BYTE(S)\nCOMPRESSED FILE SIZE: %d "
                 "BYTE(S)\nTHE %s IS COMPRESSED BY %d BYTE(S)\n",
                 size, cmp_file_size, argv[1], size - cmp_file_size);
          fclose(cmp_file);
          fclose(existing_file);
        }
      } else {
        //   flag = 1;
        printf("file doesn't exist\n");
      }
    } else if (strcmp(argv[1], "-d") == 0) {
      printf("This feature will be added soon\n");
      // FILE *existing_file = NULL;
      // existing_file = fopen(argv[2], "rb");
      // if (existing_file != NULL) {
      //   fseek(existing_file, 0, SEEK_END);
      //   size = ftell(existing_file);
      //   fseek(existing_file, 0, SEEK_SET);

      //   if (size == 0) {
      //     printf("file to decompress is empty\nnothing to decompress\n");
      //     fclose(existing_file);
      //   } else {
      //     FILE *decmp_file = NULL;
      //     decmp_file = fopen(argv[3], "wb");
      //     file_decompression(existing_file, decmp_file);
      //     fclose(decmp_file);
      //     fclose(existing_file);
      //   }

      // } else {
      //   //   flag = 1;
      //   printf("file doesn't exist\n");
      // }
    } else {
      printf("syntaxis error!\ncmptool syntaxis: cmptool -flag('c' - to "
             "compress, 'd' - to decompress) "
             "<file_to_decompress.txt> "
             "<name_of_decompressed_file.decmp>\n");
    }
  }
  return 0;
}
