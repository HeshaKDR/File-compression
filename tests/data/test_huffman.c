#include <CUnit/Basic.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "../src/huffman.h"

void test_count_frequency(void){
const char *test_input = "abcababcc";
FILE *test_file = fmemopen((void*)test_input, strlen(test_input), "r");
unsigned freq[256] = {0};

count_frequency(test_file, freq);

CU_ASSERT_EQUAL(freq['a'], 3);
CU_ASSERT_EQUAL(freq['b'], 3);
CU_ASSERT_EQUAL(freq['c'], 3);
CU_ASSERT_EQUAL(freq['f'], 0);

fclose(test_file);
}

void test_min_heapify(void){
    HuffNode node1 = {.freq = 10};
    HuffNode node2 = {.freq = 20};
    HuffNode node3 = {.freq = 30};
    HuffNode *heap[3] = {&node1, &node2, &node3};

    min_heapify(heap, 3, 0);

    CU_ASSERT_EQUAL(heap[0]->freq, 10);
    CU_ASSERT_EQUAL(heap[1]->freq, 20);
    CU_ASSERT_EQUAL(heap[2]->freq, 30);

}

void test_extract_min(void){
    HuffNode node1 = {.freq = 10};
    HuffNode node2 = {.freq = 20};
    HuffNode node3 = {.freq = 30};
    HuffNode *heap[3] = {&node1, &node2, &node3};
    int heap_size = 3;

    HuffNode *min_node = extract_min(heap, &heap_size);
    CU_ASSERT_EQUAL(min_node->freq, 10);
    CU_ASSERT_EQUAL(heap_size, 2);
}

void test_insert_heap(void){
    HuffNode node1 = {.freq = 10};
    HuffNode node2 = {.freq = 20};
    HuffNode *heap[3] = {&node1, &node2};
    int heap_size = 2;
    HuffNode node3 = {.freq = 30};

    insert_heap(heap, &heap_size, &node3);
    CU_ASSERT_EQUAL(heap[0]->freq, 10);
    CU_ASSERT_EQUAL(heap[1]->freq, 20);
    CU_ASSERT_EQUAL(heap[2]->freq, 30);
}

void test_construct_huffman(void){
    unsigned freq[256] = {0};
    freq['a'] = 1;
    freq['b'] = 2;
    freq['c'] = 3;
    freq['d'] = 4;

    HuffNode *root = NULL;
    construct_huffman(freq, &root);
    CU_ASSERT_PTR_NOT_NULL(root);
    CU_ASSERT_EQUAL(root->freq, 10);

}

void test_tree_bit_encoding(void){
unsigned char buffer =0;
int bit_count = 0;
FILE *output_test_file = tmpfile();
HuffNode node1 = {.data = 'a', .freq = 1, .is_leaf = 1};
HuffNode node2 = {.data = 'b', .freq = 2, .is_leaf = 1};
HuffNode root = {.freq = 3, .left = &node1 , .right = &node2, .is_leaf = 0};

tree_bit_encoding(&root, output_test_file, &buffer, &bit_count);

fclose(output_test_file);
CU_ASSERT_TRUE(1);
}

void test_encode_stream(void){
    unsigned freq[256] = {0};
    freq['a'] = 1;
    freq['b'] = 2;
    freq['c'] = 3;

    HuffNode *root = NULL;
    construct_huffman(freq, &root);

    FILE *fin = fmemopen("abcabc", 6, "r");
    FILE *fout = tmpfile();

    unsigned padding = 0;
    encode_stream(fin, fout, root, &padding);

    fclose(fin);
    fclose(fout);


}

int main(){
    if (CUE_SUCCESS != CU_initialize_registry()){
        printf("Cunit initialization failed");
        return EXIT_FAILURE;
    }
    CU_pSuite pSuite = CU_add_suite("Huffman Coding test\n", NULL, NULL);
    if(NULL == pSuite){
        printf("Failed to add suite\n");
        return EXIT_FAILURE;
        }
        if((NULL == CU_add_test(pSuite, "Test count_frequency", test_count_frequency)) ||
        (NULL == CU_add_test(pSuite, "Test min_heapify", test_min_heapify)) ||
        (NULL == CU_add_test(pSuite, "Test extract_min", test_extract_min)) ||
        (NULL == CU_add_test(pSuite, "Test insert_heap", test_insert_heap)) ||
        (NULL == CU_add_test(pSuite, "Test construct_huffman", test_construct_huffman)) ||
        (NULL == CU_add_test(pSuite, "Test tree_bit_encoding", test_tree_bit_encoding)) ||
        (NULL == CU_add_test(pSuite, "Test encode_stream", test_encode_stream))){
            printf("Failed to add tests\n");
            return EXIT_FAILURE;
        }

        CU_basic_set_mode(CU_BRM_VERBOSE);
        CU_basic_run_tests();

        CU_cleanup_registry();

        return CU_get_error() == CUE_SUCCESS ? EXIT_SUCCESS : EXIT_FAILURE;


}