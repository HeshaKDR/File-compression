#include "huffman.h"

void count_frequency(FILE *fp, unsigned *freq) {
  size_t original_pos = ftell(fp);
  int ch;
  while ((ch = fgetc(fp)) != EOF) {
    if (ch >= 0 && ch < 256)
      freq[ch]++;
  }
  fseek(fp, original_pos, SEEK_SET);
}

void min_heapify(HuffNode **heap, int n, int i) {
  int smallest = i;
  int left = 2 * i + 1;
  int right = 2 * i + 2;
  if (left < n && heap[left]->freq < heap[smallest]->freq) {
    smallest = left;
  }
  if (right < n && heap[right]->freq < heap[smallest]->freq) {
    smallest = right;
  }
  if (smallest != i) {
    HuffNode *temp = heap[i];
    heap[i] = heap[smallest];
    heap[smallest] = temp;
    min_heapify(heap, n, smallest);
  }
}

HuffNode *extract_min(HuffNode **heap, int *n) {
  if (*n <= 0) {
    return NULL;
  } else {
    HuffNode *minNode = heap[0];
    heap[0] = heap[--(*n)];
    min_heapify(heap, *n, 0);
    return minNode;
  }
}

void insert_heap(HuffNode **heap, int *n, HuffNode *node) {
  heap[(*n)++] = node;
  for (int i = *n - 1; i > 0; i = (i - 1) / 2) {
    if (heap[i]->freq < heap[(i - 1) / 2]->freq) {
      HuffNode *temp = heap[i];
      heap[i] = heap[(i - 1) / 2];
      heap[(i - 1) / 2] = temp;
    } else {
      break;
    }
  }
}

void construct_huffman(unsigned *freq_in, HuffNode **root) {

  HuffNode *heap[256];
  int heap_size = 0;

  for (int i = 0; i < 256; i++) {
    if (freq_in[i] > 0) {
      HuffNode *node = malloc(sizeof(HuffNode));
      node->data = i;
      node->freq = freq_in[i];
      node->left = NULL;
      node->right = NULL;
      node->parent = NULL;
      node->is_leaf = 1;
      insert_heap(heap, &heap_size, node);
    }
  }

  while (heap_size > 1) {

    HuffNode *left = extract_min(heap, &heap_size);
    HuffNode *right = extract_min(heap, &heap_size);
    HuffNode *parent = malloc(sizeof(HuffNode));
    parent->data = 0;
    parent->freq = left->freq + right->freq;
    parent->left = left;
    parent->right = right;
    parent->parent = NULL;
    parent->is_leaf = 0;

    left->parent = parent;
    right->parent = parent;
    insert_heap(heap, &heap_size, parent);
  }
  *root = (heap_size > 0) ? extract_min(heap, &heap_size) : NULL;
  printf("Huffman tree structure: \n");
  print_tree(*root, 0);
}

void print_tree(HuffNode *node, int depth) {
  if (node == NULL) {
    return;
  }
  for (int i = 0; i < depth; i++)
    printf(" ");
  if (node->is_leaf) {
    printf("Leaf: '%c' (ASCII %d), freq = %u\n", node->data, node->data,
           node->freq);

  } else {
    printf("Internal freq = %u\n", node->freq);
  }
  print_tree(node->left, depth + 1);
  print_tree(node->right, depth + 1);
}

void generate_codes(HuffNode *root, char *code, int depth, char **codes) {
  if (root == NULL)
    return;
  if (root->is_leaf) {
    code[depth] = '\0';
    codes[root->data] = strdup(code);
    return;
  }
  code[depth] = '0';
  generate_codes(root->left, code, depth + 1, codes);
  code[depth] = '1';
  generate_codes(root->right, code, depth + 1, codes);
}

void tree_bit_encoding(HuffNode *node, FILE *fout, unsigned char *buffer,
                       int *bit_count) {

  if (node == NULL)
    return;

  *buffer |= (node->is_leaf ? 0 : 1) << (7 - *bit_count);
  (*bit_count)++;
  if (*bit_count == 8) {

    fputc(*buffer, fout);
    printf("Encoded byte: %02X\n", *buffer);
    *buffer = 0;
    *bit_count = 0;
  }
  if (node->is_leaf) {
    for (int i = 0; i < 8; i++) {
      *buffer |= ((node->data >> (7 - i)) & 1) << (7 - *bit_count);
      (*bit_count)++;
      if (*bit_count == 8) {

        fputc(*buffer, fout);
        printf("Encoded byte: %02X\n", *buffer);
        *buffer = 0;
        *bit_count = 0;
      }
    }
  } else {
    tree_bit_encoding(node->left, fout, buffer, bit_count);
    tree_bit_encoding(node->right, fout, buffer, bit_count);
  }
}

void encode_stream(FILE *fin, FILE *fout, HuffNode *root, unsigned *padding) {
  char *codes[256] = {NULL};
  char code[256];
  generate_codes(root, code, 0, codes);

  int ch;
  unsigned char buffer = 0;
  int bit_count = 0;

  while ((ch = fgetc(fin)) != EOF) {
    if (ch >= 0 && ch < 256 && codes[ch] != NULL) {
      printf("Encoding '%c' (ASCII %d): %s\n", ch, ch, codes[ch]);
      for (int i = 0; codes[ch][i]; i++) {
        buffer |= (codes[ch][i] - '0') << (7 - bit_count);
        bit_count++;
        if (bit_count == 8) {
          fputc(buffer, fout);
          printf("Encode byte: %02X\n", buffer);
          buffer = 0;
          bit_count = 0;
        }
      }
    } else {
      printf("Invalid character in the file\n");
      return;
    }
  }

  if (bit_count > 0) {
    fputc(buffer, fout);
    printf("Encode byte (final) : %02X\n", buffer);
    *padding = 8 - bit_count;
  } else {
    *padding = 0;
  }
  for (int i = 0; i < 256; i++) {
    if (codes[i] != NULL) {
      free(codes[i]);
    }
  }
}

void compress_file(FILE *fin, FILE *fout) {

  unsigned freq[256] = {0}, padding = 0; //- хз

  unsigned char buffer = 0;
  int bit_count = 0;
  HuffNode *root = NULL;

  count_frequency(fin, freq);
  construct_huffman(freq, &root);

  const unsigned magic = 0x48554646;
  fwrite(&magic, sizeof(unsigned), 1, fout);
  long padding_pos = ftell(fout);

  fwrite(&padding, sizeof(unsigned), 1, fout);

  tree_bit_encoding(root, fout, &buffer, &bit_count);

  if (bit_count > 0) {
    fputc(buffer, fout);
    buffer = 0;
    bit_count = 0;
  }
  rewind(fin);
  encode_stream(fin, fout, root, &padding);
  fseek(fout, padding_pos, SEEK_SET);
  fwrite(&padding, sizeof(unsigned), 1, fout);

  free_tree(root);
}

void free_tree(HuffNode *node) {
  if (node != NULL) {
    free_tree(node->left);
    free_tree(node->right);
    free(node);
  }
}
