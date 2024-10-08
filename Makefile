FLAGS = -Wall -Werror -Wextra

all: cmptool check_file_action



cmptool: src/huffman.o src/cmptool.o
	gcc $(FLAGS) -o build/cmptool src/huffman.o src/cmptool.o


check_file_action: src/file_action.o tests/test.o
	gcc src/file_action.o tests/test.o -o build/check_file_action -lcheck -lm -lpthread -lrt -lsubunit

test.o: tests/test.c
	gcc $(FLAGS) -c tests/test.c

file_action.o: src/file_action.c
	gcc $(FLAGS) -c src/file_action.c

cmptool.o: src/cmptool.c
	gcc $(FLAGS) -c src/cmptool.c
	
huffman.o: src/huffman.c
	gcc $(FLAGS) -c src/huffman.c

clear:
	rm -f src/*.o & rm -f tests/*.o
