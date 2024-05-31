#include "s21_grep.h"

int main(int argc, char *argv[]) {
  flags num = {0};
  key patterns[100] = {'\0'};
  if (argc > 1) {
    parseFlag(&num, argc, argv);
    callOpenFile(argc, argv, &num, patterns);
  }
  return 0;
}

void parseFlag(flags *num, int argc, char **argv) {
  for (int i = 1; argc > i; i++) {
    if (argv[i][0] == '-' && argv[i][1] != '-') {
      int lenArg = strlen(argv[i]);
      int j = 1;
      while (j < lenArg) {
        if (argv[i][j] == 'e') {
          num->e = 1;
        } else if (argv[i][j] == 'i') {
          num->i = 1;
        } else if (argv[i][j] == 'v') {
          num->v = 1;
        } else if (argv[i][j] == 'c') {
          num->c = 1;
        } else if (argv[i][j] == 'l') {
          num->l = 1;
        } else if (argv[i][j] == 'n') {
          num->n = 1;
        } else if (argv[i][j] == 'h') {
          num->h = 1;
        } else if (argv[i][j] == 's') {
          num->s = 1;
        } else if (argv[i][j] == 'f') {
          num->f = 1;
        } else if (argv[i][j] == 'o') {
          num->o = 1;
        } else {
          fprintf(stderr, "grep: invalid option -- %d\n", argv[1][0]);
          break;
        }
        j++;
      }
    }
  }
}

void callOpenFile(int argc, char **argv, flags *num, key *patterns) {
  int h = 0;
  amountFiles(argc, argv, num);
  for (int i = 1; argc > i; i++) {
    if (argv[i][0] != '-') {
      if (checkCurrentTextFile(argv, i) == 0) {
        strcpy(patterns[h].word, argv[i]);
        num->amount_patterns++;
        h++;
      }
    }
  }
  if (strcmp(patterns[0].word, "\0") != 0) checkFlags(num);
  for (int i = 1; argc > i; i++) {
    if (argv[i][0] != '-' && !num->f) {
      if (checkCurrentTextFile(argv, i) != 0) {
        openFile(num, argv[i], patterns);
      }
    }
    if (argv[i][0] == '-' && checkArgFlagF(argc, argv, i) == 1) {
      flagF(argv[i + 1], num, patterns);
      int j = i;
      while (j < argc - 2) {
        if (checkCurrentTextFile(argv, j + 2) != 0) {
          openFile(num, argv[j + 2], patterns);
        }
        j++;
      }
      int k = 1;
      while (k < i) {
        if (checkCurrentTextFile(argv, k) != 0) {
          openFile(num, argv[k], patterns);
        }
        k++;
      }
    }
  }
}

void amountFiles(int argc, char **argv, flags *num) {
  for (int i = 1; argc > i; i++) {
    if (argv[i][0] != '-') {
      if (checkCurrentTextFile(argv, i) != 0) {
        num->amount_Files++;
      }
    }
  }
  if (num->f) num->amount_Files--;
}

int checkCurrentTextFile(char **argv, int numb) {
  int count = 0;
  if (argv[numb][0] != '-') {
    if (strstr(argv[numb], ".txt") != NULL) count++;
    if (strstr(argv[numb], ".c") != NULL) count++;
    if (strstr(argv[numb], ".h") != NULL) count++;
    if (strstr(argv[numb], ".file") != NULL) count++;
    if (strstr(argv[numb], "Makefile") != NULL) count++;
    if (strstr(argv[numb], "makefile") != NULL) count++;
  }
  return count;
}

void checkFlags(flags *num) {
  if (!num->e && !num->i && !num->v && !num->c && !num->l && !num->n &&
      !num->h && !num->s && !num->f && !num->o)
    num->e = 1;
}

int checkArgFlagF(int argc, char **argv, int numb) {
  int argFlagF = 0;
  for (int i = 1; argc > i; i++) {
    if (argv[numb][0] == '-' && argv[numb][1] != '-') {
      int lenArg = strlen(argv[numb]);
      int j = 1;
      while (j < lenArg) {
        if (argv[numb][j] == 'f') {
          argFlagF = 1;
        }
        j++;
      }
    }
  }
  return argFlagF;
}

void openFile(flags *num, char *file_name, key *patterns) {
  FILE *f;
  char buffer[1000];
  f = fopen(file_name, "r");
  if (f != NULL) {
    int retval;
    int countFlagC = 0;
    int lines_count = 1;
    int amountPrint = 1;
    int countFlagL = 1;
    while (fgets(buffer, sizeof(buffer), f) != NULL) {
      if (num->i) {
        retval = match(buffer, num, patterns, REG_ICASE);
      } else {
        retval = match(buffer, num, patterns, REG_EXTENDED);
      }
      amountPrint = printFlagF(num, retval, buffer, amountPrint);
      countFlagC = flagC(num, retval, countFlagC, countFlagL);
      countFlagL = flagL(file_name, num, retval, countFlagL);
      amountPrint =
          flagN(file_name, num, retval, buffer, lines_count, amountPrint);
      flagNO(file_name, num, retval, lines_count, amountPrint);
      amountPrint = flagV(file_name, num, retval, buffer, amountPrint);
      amountPrint = flagE(file_name, num, retval, buffer, amountPrint);
      amountPrint = flagH(num, retval, buffer, amountPrint);
      amountPrint = flagS(num, retval, buffer, amountPrint);
      amountPrint = flagI(num, retval, buffer, amountPrint);
      flagO(buffer, num, patterns, file_name);
      if (buffer[strlen(buffer) - 1] != '\n' && amountPrint != 1) printf("\n");
      amountPrint = 1;
      lines_count++;
    }
    printFlagC(file_name, num, countFlagC, countFlagL);
    countFlagL = 1;
  }
  fclose(f);
}

int match(char *string, flags *num, key *patterns, int regFlag) {
  int status;
  regex_t re;
  int result = 1;
  for (int i = 0; i < num->amount_patterns; i++) {
    regcomp(&re, patterns[i].word, regFlag);
    if ((status = regexec(&re, string, 0, NULL, 0)) == 0) {
      result = status;
    }
    regfree(&re);
  }
  return result;
}

int flagE(char *file_name, flags *num, int retval, char *buffer,
          int amountPrint) {
  if (num->e && !num->c && !num->i && !num->l && !num->h && amountPrint == 1) {
    if (num->amount_Files == 1 || num->h) {
      if (retval == 0 && !num->v) {
        printf("%s", buffer);
        amountPrint++;
      }
      if (retval != 0 && num->v) {
        printf("%s", buffer);
        amountPrint++;
      }
    } else {
      if (retval == 0 && !num->v) {
        printf("%s:%s", file_name, buffer);
        amountPrint++;
      }
      if (retval != 0 && num->v) {
        printf("%s:%s", file_name, buffer);
        amountPrint++;
      }
    }
  }
  return amountPrint;
}

int flagI(flags *num, int retval, char *buffer, int amountPrint) {
  if (num->i && !num->n && !num->c && amountPrint == 1) {
    if (num->v && retval != 0) {
      printf("%s", buffer);
      amountPrint++;
    }
    if (!num->v && retval == 0) {
      printf("%s", buffer);
      amountPrint++;
    }
  }
  return amountPrint;
}

int flagV(char *file_name, flags *num, int retval, char *buffer,
          int amountPrint) {
  if (num->v && retval != 0 && !num->e && !num->c && !num->i && !num->l &&
      !num->h && !num->f && amountPrint == 1) {
    if (num->amount_Files == 1) {
      printf("%s", buffer);
      amountPrint++;
    } else {
      printf("%s:%s", file_name, buffer);
      amountPrint++;
    }
  }
  return amountPrint;
}

int flagC(flags *num, int retval, int countFlagC, int countFlagL) {
  if (num->c) {
    if (retval == 0 && !num->v) countFlagC++;
    if (retval != 0 && num->v) countFlagC++;
    if (num->l && countFlagC > 0 && countFlagL == 1) {
      printf("%d\n", countFlagC);
      countFlagC = 1;
    }
  }
  return countFlagC;
}

int flagL(char *file_name, flags *num, int retval, int countFlagL) {
  if (num->l) {
    if (retval == 0 && !num->v && countFlagL == 1) {
      printf("%s\n", file_name);
      countFlagL++;
    }
    if (retval != 0 && num->v && countFlagL == 1) {
      printf("%s\n", file_name);
      countFlagL++;
    }
  }
  return countFlagL;
}

int flagN(char *file_name, flags *num, int retval, char *buffer,
          int lines_count, int amountPrint) {
  if (num->n && amountPrint == 1 && !num->c && !num->l && !num->o) {
    if (num->amount_Files == 1 || num->h) {
      if (retval == 0 && !num->v) {
        printf("%d:%s", lines_count, buffer);
        amountPrint++;
      }
      if (retval != 0 && num->v) {
        printf("%d:%s", lines_count, buffer);
        amountPrint++;
      }
    } else {
      if (retval == 0 && !num->v) {
        printf("%s:%d:%s", file_name, lines_count, buffer);
        amountPrint++;
      }
      if (retval != 0 && num->v) {
        printf("%s:%d:%s", file_name, lines_count, buffer);
        amountPrint++;
      }
    }
  }
  return amountPrint;
}

void flagNO(char *file_name, flags *num, int retval, int lines_count,
            int amountPrint) {
  if (num->n && amountPrint == 1 && !num->c && num->o && !num->l) {
    if (num->amount_Files == 1 || num->h) {
      if (retval == 0 && !num->v) {
        printf("%d:", lines_count);
      }
      if (retval != 0 && num->v) {
        printf("%d:", lines_count);
      }
    } else {
      if (retval == 0 && !num->v) {
        printf("%s:%d:", file_name, lines_count);
      }
      if (retval != 0 && num->v) {
        printf("%s:%d:", file_name, lines_count);
      }
    }
  }
}

int flagH(flags *num, int retval, char *buffer, int amountPrint) {
  if (num->h && !num->n && !num->c && !num->o && !num->l && amountPrint == 1) {
    if (retval == 0 && !num->v) {
      printf("%s", buffer);
      amountPrint++;
    }
    if (retval != 0 && num->v) {
      printf("%s", buffer);
      amountPrint++;
    }
  }
  return amountPrint;
}

int flagS(flags *num, int retval, char *buffer, int amountPrint) {
  if (num->s && retval == 0 && !num->v && !num->c && !num->i && !num->l &&
      !num->h && !num->f && !num->o && amountPrint == 1) {
    printf("%s", buffer);
    amountPrint++;
  }
  return amountPrint;
}

void flagF(char *file_name, flags *num, key *patterns) {
  if (num->f) {
    FILE *file;
    char buffer2[1000];
    int i = num->amount_patterns;
    file = fopen(file_name, "r");
    if (file != NULL) {
      while (fgets(buffer2, sizeof(buffer2), file) != NULL) {
        strcpy(patterns[i].word, buffer2);
        i++;
      }
      num->amount_patterns = i;
    }
    fclose(file);
  }
}

void flagO(char *string, flags *num, key *patterns, char *file_name) {
  if (num->o && !num->c && !num->v && !num->l) {
    regex_t re;
    regmatch_t pmatch[100];
    int status;
    char *ps = NULL;
    ps = string;
    for (int j = 0; j < num->amount_patterns; j++) {
      if (num->i) {
        status = regcomp(&re, patterns[j].word, REG_ICASE);
      } else {
        status = regcomp(&re, patterns[j].word, REG_EXTENDED);
      }
      while ((status = regexec(&re, ps, 1, pmatch, 0)) == 0) {
        if (num->amount_Files != 1 && !num->h) printf("%s:", file_name);
        for (int i = pmatch[0].rm_so; i < pmatch[0].rm_eo; i++)
          printf("%c", ps[i]);
        ps += pmatch[0].rm_eo;
        printf("\n");
      }
      regfree(&re);
    }
  }
}

void printFlagC(char *file_name, flags *num, int countFlagC, int countFlagL) {
  if (num->c && !num->l) {
    if (num->amount_Files == 1 || num->h)
      printf("%d\n", countFlagC);
    else
      printf("%s:%d\n", file_name, countFlagC);
  }
  if (num->c && num->l && countFlagL == 1) {
    if (num->amount_Files == 1 || num->h)
      printf("%d\n", countFlagC);
    else
      printf("%s:%d\n", file_name, countFlagC);
  }
}

int printFlagF(flags *num, int retval, char *buffer, int amountPrint) {
  if (num->f && num->amount_patterns != 0 && !num->e && !num->c && !num->l &&
      !num->n && !num->h && !num->o && amountPrint == 1) {
    if (retval == 0) {
      printf("%s", buffer);
      amountPrint++;
    }
  }
  return amountPrint;
}