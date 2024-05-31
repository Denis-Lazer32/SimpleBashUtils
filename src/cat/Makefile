CC = gcc
FLAGS = -std=c11 -Wall -Werror -Wextra
CAT = s21_cat

s21_cat : s21_cat.c

	$(CC) $(FLAGS) -c s21_cat.c

	$(CC) s21_cat.o -o $(CAT)

	rm s21_cat.o

clean :

	rm s21_cat

rebuild :

	make clean

	make s21_cat
