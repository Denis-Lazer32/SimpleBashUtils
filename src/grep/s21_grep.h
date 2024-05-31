#ifndef S21_GREP_H
#define S21_GREP_H

#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int f;
  int o;
  int amount_patterns;
  int amount_Files;
} flags;

typedef struct {
  char word[100];
} key;

void printPattern(flags *num, key *patterns);
void printFiles(int argc, char **argv);
void parseFlag(flags *num, int argc, char **argv);
int checkCurrentTextFile(char **argv, int numb);
int checkArgFlagF(int argc, char **argv, int numb);
int match(char *string, flags *num, key *patterns, int regFlag);
void openFile(flags *num, char *file_name, key *patterns);
int printFlagF(flags *num, int retval, char *buffer, int amountPrint);
int flagC(flags *num, int retval, int countFlagC, int countFlagL);
int flagL(char *file_name, flags *num, int retval, int countFlagL);
int flagN(char *file_name, flags *num, int retval, char *buffer,
          int lines_count, int amountPrint);
void flagNO(char *file_name, flags *num, int retval, int lines_count,
            int amountPrint);
int flagV(char *file_name, flags *num, int retval, char *buffer,
          int amountPrint);
int flagE(char *file_name, flags *num, int retval, char *buffer,
          int amountPrint);
int flagH(flags *num, int retval, char *buffer, int amountPrint);
int flagS(flags *num, int retval, char *buffer, int amountPrint);
int flagI(flags *num, int retval, char *buffer, int amountPrint);
void flagF(char *file_name, flags *num, key *patterns);
void flagO(char *string, flags *num, key *patternschar, char *file_name);
void printFlagC(char *file_name, flags *num, int countFlagC, int countFlagL);
void amountFiles(int argc, char **argv, flags *num);
void checkFlags(flags *num);
void callOpenFile(int argc, char **argv, flags *num, key *patterns);

#endif