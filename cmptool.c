#include <stdio.h>

int main(int argc, char *argv[]) {
  int i = 2;
  int flag = 0;

  if (argc > i) {
    FILE *existing_file;
    FILE *cmp_file;
    existing_file = fopen(argv[1], "r");
    if (existing_file == NULL) {
      flag = 1;
      printf("file doesn't exist");
      fclose(existing_file);
    }

    if (flag == 0) {
      cmp_file = fopen(argv[2], "w");
      fclose(cmp_file);
    }
  }

  else {
    printf("syntaxis error!\ncmptool syntaxis: cmptool <example.txt> "
           "<example.cmp>\n");
  }
  return flag;
}