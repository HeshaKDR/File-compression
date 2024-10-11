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

void construct_huffman(unsigned *freq_in, HuffNode *tree) {
  int count = 256; //установка счетчика до 256
  unsigned freq[256] = {0}; // создания массива для повторяющитхся значений 
  HuffNode *node[256];  //выделение памяти под узлы дерева 

  for (int i = 0; i < 256; i++) { //включаем цикл
    freq[i] = freq_in[i]; //переписываем массив (с частотой повторений) который получили в функции count_freq во вновь созданный массив freq - каждму узлу присваевается соотвествующая частота повторений, выявленная в функции
    tree[i].data = i; // задаем порядковый номер каждому узлу по ходу перебора массива 
    tree[i].left = NULL;
    tree[i].right = NULL;
    tree[i].parent = NULL;
    tree[i].is_leaf = 1; //ставим указатели в узле на NULL
    node[i] = &tree[i]; //присваеваем адрес узла дерева к созданному узлу
  }
  for (int i = 0; i < 256; i++) {  //делаем сортировку пузырьком полученного массива 
    for (int j = 0; j < 256 - i - 1; j++) {
      if (j + 1 < 256 && (freq[j] < freq[j + 1] || (freq[j] == freq[j+1] && j < j + 1))) { // условие что если значение больше впередиидущего значения - они меняются местами 
        unsigned t = freq[j];
        freq[j] = freq[j + 1];
        freq[j + 1] = t;
        HuffNode *p = node[j]; // такое же происходит и с порядком узлов в дереыве
        node[j] = node[j + 1];
        node[j + 1] = p;
      }
    }
  }

  while (count > 1) {
    int pos = 512 - count;
    HuffNode *parent = &tree[pos];
    int i = count - 2, j = count - 1;
    parent->left = node[j]; //присваеваем левому узлу ячейку узла j
    parent->right = node[i]; //присваеваем правому узлу ячейку узла i
    node[j]->parent = parent;
    node[i]->parent = parent;
    node[i]->is_leaf = 0;
    node[j]->is_leaf = 0;
    node[i] = parent;
    freq[i] += freq[j]; //происходит сложение частот повторений
    for (; i > 0 && freq[i] > freq[i - 1]; i--) { //продолжение сортировки узлов но теперьб уже с вновь обращованным узлом
      unsigned t = freq[i];
      freq[i] = freq[i - 1];
      freq[i - 1] = t;
      HuffNode *p = node[i];
      node[i] = node[i - 1];
      node[i - 1] = p;
    }
    count--;
  }
  node[0]->parent = NULL;
}

void tree_bit_encoding(HuffNode *node, FILE *fout, unsigned char *buffer, int *bit_count){
  if(node->is_leaf){
    *buffer |= (0 << *bit_count);
    (*bit_count)++;
    if(*bit_count == 8){
      
      fputc(*buffer, fout);
      *buffer = 0;
      bit_count = 0;
      }
      for(int i = 7; i >= 0; i--){
        *buffer |= ((node->data >> i) & 1) << *bit_count;
        (*bit_count)++;
        if(*bit_count == 8){
          
          fputc(*buffer, fout);
          *buffer = 0;
          *bit_count = 0;        }
      }
  }
  else{
    *buffer |= (1 << *bit_count);
    (*bit_count) ++;
    if(*bit_count == 8){
      fputc(*buffer, fout);
      *buffer = 0;
      *bit_count = 0;

    }
    tree_bit_encoding(node->left, fout, buffer, bit_count);
    tree_bit_encoding(node->right, fout, buffer, bit_count);
  }
}



void encode_stream(FILE *fin, FILE *fout, HuffNode *tree, unsigned *padding) {
  int ch;
  unsigned char buffer = 0;
  int bit_count = 0;
  HuffNode *p;
  unsigned char code[256] = {0};
  while ((ch = fgetc(fin)) != EOF) {
    if(ch >= 0 && ch < 256){
      
    
    p = &tree[ch];
    
    
    int code_length = 0;
    while (p->parent) {
      if (p == p->parent->right) {
        code[code_length] = 1;
      }
      else{
        code[code_length] = 0;
      }
      p = p->parent;
      code_length++;
    }

    for (int i = code_length - 1; i >= 0; i--) {

      buffer |= code[i] << bit_count;
      
      bit_count++;
      if (bit_count == 8) {
        
        fputc(buffer, fout);
        bit_count = 0;
        buffer = 0;
      }
    }
  } else{
    printf("invalid characters in the file");
      return;
    
  }
  
}
if (bit_count > 0) {
    fputc(buffer, fout);
  }
  *padding = 8 - bit_count;
}

void compress_file(FILE *fin, FILE *fout) {

  unsigned freq[256] = {0}, padding = 0; //- хз
  
  unsigned char buffer = 0;
  int bit_count = 0;
  HuffNode tree[512];

  
    count_frequency(fin, freq);
    construct_huffman(freq, tree);
    rewind(fin);
    tree_bit_encoding(&tree[511], fout, &buffer, &bit_count);
    
    if(bit_count > 0){
      fputc(buffer, fout);
    }
    encode_stream(fin, fout, tree, &padding);
    
    fwrite(&padding, sizeof(unsigned), 1, fout);

  }
