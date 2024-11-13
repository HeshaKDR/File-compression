#include "huffman_decompression.h"


HuffNode *rebuild_tree(FILE *fin, unsigned char *buffer, int *bit_count) {
  
  if (*bit_count == 0) {
    *buffer = fgetc(fin);
    *bit_count = 8;
  }
  (*bit_count)--;
  int bit = (*buffer >> *bit_count) & 1;

  HuffNode *node = malloc(sizeof(HuffNode));
  if (node == NULL) {
    printf("Program broke!\nPlease try again\n");
    return NULL;
  }
  
    if (bit == 0) {
      node->data = 0;
      for (int i = 0; i < 8; i++) {
        if (*bit_count == 0) {
          *buffer = fgetc(fin);
          *bit_count = 8;
        }
        (*bit_count)--;
        node->data |= ((*buffer >> *bit_count) & 1) << (7 - i);
      }
      node->left = NULL;
      node->right = NULL;
      node->is_leaf = 1;

    } else {
      node->is_leaf = 0;
      node->left = rebuild_tree(fin, buffer, bit_count);
      node->right = rebuild_tree(fin, buffer, bit_count);
    }
  
  return node;
}

void decode_stream(FILE *fin, FILE *fout, HuffNode *root, unsigned *padding) {
  HuffNode *current = root;
  unsigned char buffer = 0;
  int total_bits = 0;
  int bits_read = 0;
  long file_size = 0;
  long initial_pos = ftell(fin);

  fseek(fin, 0, SEEK_END);
  file_size = ftell(fin);
  fseek(fin, initial_pos, SEEK_SET);

    total_bits = (file_size - initial_pos) * 8 - *padding;
    while (bits_read < total_bits) {
      if (fread(&buffer, 1, 1, fin) != 1) {
        break;
      }
      for(int i = 7; i>=0 && bits_read < total_bits; i--){
        bits_read++;
        int bit = (buffer>>i) & 1;

        if(bit == 0){
          current = current->left;
        }else{
          current = current->right;
        }
        if(current->is_leaf){
          fputc(current->data, fout);
          current = root;
        }
        }
        }
      }

void file_decompression(FILE *fin, FILE *fout) {
  unsigned char buffer = 0;
  int bit_count = 0;
  unsigned magic;
if(fread(&magic, sizeof(unsigned), 1, fin) != 1 || magic != 0x48554646){
  printf("invalid file format\n");
  return;
}

unsigned padding;
  if(fread(&padding, sizeof(unsigned), 1, fin) != 1){
    printf("Failed to read padding information\n");
    return;
  }
  HuffNode *root = rebuild_tree(fin, &buffer, &bit_count);
  if(root == NULL){
    printf("Failed to rebuild Huffman tree\n");
    return;
  }
  
  decode_stream(fin, fout, root, &padding);
  free_tree(root);
}