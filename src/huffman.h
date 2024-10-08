#ifndef HUFFMAN_H
#define HUFFMAN_H
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdio.h>

struct HuffNode{
    unsigned data;
    struct HuffNode *left;
    struct HuffNode *right;
    struct HuffNode *parent;
    int is_leaf;
};
typedef struct HuffNode HuffNode;

void count_frequency(FILE * fp, unsigned *freq);
void construct_huffman(unsigned *freq_in, HuffNode *tree);
void encode_stream(FILE *fin, FILE *fout, HuffNode *tree, unsigned *padding);
void compress_file(FILE *fin, FILE *fout);
#endif