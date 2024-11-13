FLAGS = -Wall -Werror -Wextra

all: clear copy cmptool

copy:
	cp -r ./src build/
clear:
	rm -rf build/src build/cmptool build/tests

cmptool: huffman.o cmptool.o huffman_decompression.o
	gcc $(FLAGS) -o build/cmptool build/src/huffman.o build/src/cmptool.o build/src/huffman_decompression.o 

cmptool.o:
	gcc $(FLAGS) -c build/src/cmptool.c -o build/src/cmptool.o
	
huffman.o:
	gcc $(FLAGS) -c build/src/huffman.c -o build/src/huffman.o

huffman_decompression.o:
	gcc $(FLAGS) -c build/src/huffman_decompression.c -o build/src/huffman_decompression.o

file_action.o:
	gcc $(FLAGS) -c build/src/file_action.c -o build/src/file_action.o


tests: copy_tests check_file_action

copy_tests:
	cp -r ./tests build/

check_file_action: test.o file_action.o
	gcc build/src/file_action.o build/tests/test.o -o build/check_file_action -lcheck -lm -lpthread -lrt -lsubunit

test.o: 
	gcc $(FLAGS) -c build/tests/test.c -o build/tests/test.o
