CC = gcc
FLAGS = -std=c11 -Wall -Werror -Wextra
GREP = s21_grep

s21_grep : s21_grep.c

	$(CC) $(FLAGS) -c s21_grep.c

	$(CC) s21_grep.o -o $(GREP)

	rm s21_grep.o

clean :

	rm s21_grep


rebuild :

	make clean

	make s21_grep
