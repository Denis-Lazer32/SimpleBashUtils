#ifndef S21_CAT_H
#define S21_CAT_H

#include <ctype.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define FILE_ERR printf("Error in file: %s", argv[optind])
#define END (-1)
#define DEL 127
#define SHIFT 64
#define MAX_SLASH_N 3

typedef struct {
  bool b;
  bool E;
  bool n;
  bool s;
  bool T;
  bool v;
  bool h;
} config;

static const char SHORT_FLAGS[] = "AbeEnstTv";
static const struct option LONG_FLAGS[] = {
    {"number-nonblank", no_argument, NULL, 'b'},
    {"number", no_argument, NULL, 'n'},
    {"squeeze-blank", no_argument, NULL, 's'},
    {"show-ends", no_argument, NULL, 'E'},
    {"show-tabs", no_argument, NULL, 'T'},
    {"show-nonprinting", no_argument, NULL, 'v'},
    {0, 0, 0, 0}};

bool Parse(int argc, char *argv[], config *flags);
bool ProcessFile(char *filename, config flags, char *prev, int *numeracy,
                 int *cnt_slash_n);
void ProcessSymbol(char cur, config flags, const char *prev, int *numeracy,
                   int *cnt_slash_n);

#endif
