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

void encode_stream(FILE *fin, FILE *fout, HuffNode *tree, unsigned *padding) {
  int n;
  unsigned char ch;
  unsigned char buff = 0, nbuf = 0;
  HuffNode *p;
  unsigned char code[256] = {0};
  while ((n = fgetc(fin)) != EOF) {
    if(n < 0 || n >= 256){
      printf("invalid characters in the file");
      return;
    }
    ch = n;
    p = &tree[ch];
    int code_length = 0;
    while (p->parent) {
      if (p == p->parent->right) {
        code[code_length] = 1;
      }
      p = p->parent;
      code_length++;
    }
    for (int i = code_length - 1; i >= 0; i--) {
      buff |= code[i] << nbuf;
      nbuf++;
      if (nbuf == 8) {
        fputc(buff, fout);
        nbuf = 0;
        buff = 0;
      }
    }
  }
  if (nbuf > 0) {
    fputc(buff, fout);
  }
  *padding = 8 - nbuf;
}

void compress_file(FILE *fin, FILE *fout) {

  unsigned freq[256] = {0}, padding = 0; //- хз
  int flag = 0;
  size_t padding_pos;
  HuffNode tree[512]; //нахуя тут нужно выделение памяти под дерево если выделяется память под сами узлы...
  
  if (flag == 0) {
    count_frequency(fin, freq);
    construct_huffman(freq, tree);
    rewind(fin);
    for (int i = 0; i < 256; i++) {
      fwrite(&freq[i], sizeof(unsigned), 1, fout);
    }
    padding_pos = ftell(fout);
    fwrite(&padding, sizeof(unsigned), 1, fout);
    encode_stream(fin, fout, tree, &padding);
    fseek(fout, padding_pos, SEEK_SET);
    fwrite(&padding, sizeof(unsigned), 1, fout);
    
  }
}