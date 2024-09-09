FLAGS = -Wall -Werror -Wextra

all: cmptool check_file_action



cmptool: file_action.o cmptool.o
	gcc $(FLAGS) -o cmptool file_action.o cmptool.o


check_file_action: file_action.o test.o
	gcc file_action.o test.o -o check_file_action -lcheck -lm -lpthread -lrt -lsubunit

test.o: ../tests/test.c
	gcc $(FLAGS) -c ../tests/test.c

file_action.o: file_action.c
	gcc $(FLAGS) -c file_action.c

cmptool.o: cmptool.c
	gcc $(FLAGS) -c cmptool.c

clear:
	rm -f *.o
