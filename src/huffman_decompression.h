#ifndef HUFFMAN_DECOMPRESSION_H
#define HUFFMAN_DECOMPRESSION_H
#include "huffman.h"
HuffNode *rebuild_tree(FILE *fin, unsigned char *buffer, int *bit_count);
void decode_stream(FILE *fin, FILE *fout, HuffNode *root, unsigned *padding);
void file_decompression(FILE *fin, FILE *fout);
void free_tree(HuffNode *node);
#endif