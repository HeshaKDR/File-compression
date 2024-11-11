#include "huffman_decompression.h"


HuffNode *rebuild_tree(FILE *fin, unsigned char *buffer, int *bit_count) {
  int flag = 0;
  if (*bit_count == 0) {
    *buffer = fgetc(fin);
    *bit_count = 8;
  }
  (*bit_count)--;
  int bit = (*buffer >> *bit_count) & 1;
  *buffer &= (1 << *bit_count) - 1;

  HuffNode *node = malloc(sizeof(HuffNode));
  if (node == NULL) {
    printf("Program broke!\nPlease try again\n");
    flag = 1;
  }
  if (flag == 0) {
    if (bit == 0) {
      node->data = 0;
      for (int i = 0; i < 8; i++) {
        if (*bit_count == 0) {
          *buffer = fgetc(fin);
          *bit_count = 8;
        }
        (*bit_count)--;
        node->data |= ((*buffer >> *bit_count) & 1) << (7 - i);
        *buffer &= (1 << *bit_count) - 1;
      }
      node->left = NULL;
      node->right = NULL;
      node->is_leaf = 1;

    } else {
      node->left = rebuild_tree(fin, buffer, bit_count);
      node->right = rebuild_tree(fin, buffer, bit_count);
      node->is_leaf = 0;
    }
  }
  return node;
}

void decode_stream(FILE *fin, FILE *fout, HuffNode *root, unsigned *padding) {
  HuffNode *current = root;
  unsigned char buffer = 0;
  int bit_count = 0;
  int total_bits = 0;
  long file_size = 0;

  fseek(fin, 0, SEEK_SET);
  file_size = ftell(fin) * 8 - padding;
  fseek(fin, -(file_size / 8 + (file_size % 8 != 0)), SEEK_CUR);

    while (total_bits < file_size) {
    if (bit_count == 0) {
      if (fread(&buffer, 1, 1, fin) != 1) {
        break;
      }
      bit_count = 8;
    }
    bit_count--;
    total_bits++;
    int bit = (buffer >> bit_count) & 1;
    if (bit == 0) {
      current = current->left;
    } else {
      current = current->right;
    }
    if (current->is_leaf) {
      
        fputc(current->data, fout);
      
      current = root;
    }
  }
}

void file_decompression(FILE *fin, FILE *fout) {
  unsigned char buffer = 0;
  int bit_count = 0;

unsigned padding;
  fread(&padding, sizeof(unsigned), 1, fin);
  HuffNode *root = rebuild_tree(fin, &buffer, &bit_count);
  
  decode_stream(fin, fout, root, padding);
  free_tree(root);
}

void free_tree(HuffNode *node) {
  
  if (node != NULL) {
    free_tree(node->left);
    free_tree(node->right);
    free_tree(node);
  }
}