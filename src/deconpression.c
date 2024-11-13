#include "huffman.h"
#include <stdlib.h>

HuffNode* rebuild_tree(FILE *fin, unsigned char *buffer, int *bit_count) {
    if (*bit_count == 0) {
        *buffer = fgetc(fin);
        *bit_count = 8;
    }

    (*bit_count)--;
    int bit = (*buffer >> *bit_count) & 1;
    *buffer &= (1 << *bit_count) - 1;

    HuffNode *node = (HuffNode*)malloc(sizeof(HuffNode));
    if (node == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    if (bit == 0) {  // Leaf node
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
        node->left = node->right = NULL;
        node->is_leaf = 1;
    } else {  // Internal node
        node->left = rebuild_tree(fin, buffer, bit_count);
        node->right = rebuild_tree(fin, buffer, bit_count);
        node->is_leaf = 0;
    }

    return node;
}

void decode_stream(FILE *fin, FILE *fout, HuffNode *root, unsigned padding) {
    HuffNode *current = root;
    unsigned char buffer = 0;
    int bit_count = 0;
    int total_bits = 0;
    int eof_reached = 0;

    while (!eof_reached) {
        if (bit_count == 0) {
            if (fread(&buffer, 1, 1, fin) != 1) {
                eof_reached = 1;
                continue;
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
            if (total_bits + padding <= ftell(fin) * 8) {
                fputc(current->data, fout);
            }
            current = root;
        }
    }
}

void decompress_file(FILE *fin, FILE *fout) {
    unsigned char buffer = 0;
    int bit_count = 0;
    
    // Rebuild the Huffman tree
    HuffNode *root = rebuild_tree(fin, &buffer, &bit_count);

    // Read padding
    unsigned padding;
    fread(&padding, sizeof(unsigned), 1, fin);

    // Decode the stream
    decode_stream(fin, fout, root, padding);

    // Free the Huffman tree
    free_tree(root);
}

// Helper function to free the Huffman tree
void free_tree(HuffNode *node) {
    if (node == NULL) return;
    free_tree(node->left);
    free_tree(node->right);
    free(node);
}