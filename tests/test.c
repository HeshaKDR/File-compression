#include <check.h>
#include "../src/file_action.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

START_TEST(test_file_rewriting){

    int buffer_size = 128;
    const char *input_data = "Test-Test";

    FILE *input_file = tmpfile();
    FILE *output_file = tmpfile();

    fwrite(input_data, sizeof(char), strlen(input_data), input_file);
    fseek(input_file, 0, SEEK_SET);

    file_rewriting(input_file, output_file);
    fseek(output_file, 0, SEEK_SET);

    char buffer[buffer_size];
    size_t bytes_read = fread(buffer, sizeof(char), buffer_size, output_file);
    buffer[bytes_read] = '\0';

    ck_assert_str_eq(buffer, input_data);

    fclose(input_file);
    fclose(output_file);

}END_TEST

Suite *file_action_suite(void){
    Suite *s;
    TCase *tc_core;

    s = suite_create("File Actions");
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_file_rewriting);
    suite_add_tcase(s, tc_core);
    
    return s;
}


int main(void){
    int n_failed = 0;
    Suite *s;
    SRunner *runner;
    s = file_action_suite();
    runner = srunner_create(s);
    srunner_run_all(runner, CK_NORMAL);
    n_failed = srunner_ntests_failed(runner);
    srunner_free(runner);
    return(n_failed = 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}