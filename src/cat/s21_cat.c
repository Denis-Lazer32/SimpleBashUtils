#include "s21_cat.h"

int main(int argc, char *argv[]) {
  config flags = {false};
  int exit_code = EXIT_SUCCESS;
  if (!Parse(argc, argv, &flags) || optind == argc) {
    printf("ERROR");
    exit_code = EXIT_FAILURE;
  } else {
    int numeracy = 0, cnt_slash_n = 1;
    char prev = '\n';
    for (; optind < argc; ++optind)
      if (!ProcessFile(argv[optind], flags, &prev, &numeracy, &cnt_slash_n))
        FILE_ERR;
  }
  return exit_code;
}

bool Parse(int argc, char *argv[], config *flags) {
  bool valid = true;
  int service = 0, cur_flag;
  while ((cur_flag = getopt_long(argc, argv, SHORT_FLAGS, LONG_FLAGS,
                                 &service)) != END &&
         valid) {
    switch (cur_flag) {
      case 'A':
        flags->v = true;
        flags->E = true;
        flags->T = true;
        break;
      case 'b':
        flags->b = true;
        break;
      case 'e':
        flags->E = true;
        flags->v = true;
        break;
      case 'E':
        flags->E = true;
        break;
      case 'n':
        flags->n = true;
        break;
      case 's':
        flags->s = true;
        break;
      case 't':
        flags->T = true;
        flags->v = true;
        break;
      case 'T':
        flags->T = true;
        break;
      case 'v':
        flags->v = true;
        break;
      default:
        valid = false;
        break;
    }
  }
  if (flags->b) flags->n = false;
  return valid;
}

bool ProcessFile(char *filename, config flags, char *prev, int *numeracy,
                 int *cnt_slash_n) {
  bool file_is_valid = true;
  FILE *f = fopen(filename, "r");
  if (!f)
    file_is_valid = false;
  else {
    char cur = fgetc(f);
    while (!feof(f)) {
      ProcessSymbol(cur, flags, prev, numeracy, cnt_slash_n);
      *prev = cur;
      cur = fgetc(f);
    }
    fclose(f);
  }
  return file_is_valid;
}

void ProcessSymbol(char cur, config flags, const char *prev, int *numeracy,
                   int *cnt_slash_n) {
  if (flags.s) cur == '\n' ? ++((*cnt_slash_n)) : ((*cnt_slash_n) = 0);
  if (cur != '\n' || *cnt_slash_n < MAX_SLASH_N) {
    if ((flags.b && *prev == '\n' && cur != '\n') ||
        (flags.n && *prev == '\n')) {
      printf("%6d\t", ++(*numeracy));
    }
    if (flags.E && cur == '\n') {
      putchar('$');
    }
    if (isprint(cur) || (cur == '\t' && flags.T == false) || cur == '\n')
      putchar(cur);
    else if (cur == '\t' && flags.T)
      printf("^I");
    else if (iscntrl(cur)) {
      if (flags.v)
        cur == DEL ? printf("^%c", cur - SHIFT) : printf("^%c", cur + SHIFT);
      else
        putchar(cur);
    } else {
      if (flags.v) {
        char meta = (signed char)cur + DEL + 1;
        printf("M-");
        if (isprint(meta))
          putchar(meta);
        else
          meta == DEL ? printf("^%c", meta - SHIFT)
                      : printf("^%c", meta + SHIFT);
      } else
        putchar(cur);
    }
  }
}
