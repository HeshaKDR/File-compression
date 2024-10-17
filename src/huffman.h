#ifndef HUFFMAN_H
#define HUFFMAN_H
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct HuffNode {
  unsigned data;
  unsigned freq;
  struct HuffNode *left;
  struct HuffNode *right;
  struct HuffNode *parent;
  int is_leaf;
};
typedef struct HuffNode HuffNode;

void count_frequency(FILE *fp, unsigned *freq);
void min_heapify(HuffNode **heap, int n, int i);
HuffNode *extract_min(HuffNode **heap, int *n);
void insert_heap(HuffNode **heap, int *n, HuffNode *node);

void construct_huffman(unsigned *freq_in, HuffNode **root);
void print_tree(HuffNode *node, int depth);
void generate_codes(HuffNode *root, char *code, int depth, char **codes);
void tree_bit_encoding(HuffNode *node, FILE *fout, unsigned char *buffer,
                       int *bit_count);
void encode_stream(FILE *fin, FILE *fout, HuffNode *tree, unsigned *padding);
void compress_file(FILE *fin, FILE *fout);
void free_tree(HuffNode *node);
#endif