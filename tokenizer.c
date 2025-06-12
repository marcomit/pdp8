#include "tokenizer.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

static bool is_important(char c) { return isalnum(c) || c == ',' || c == ' '; }

static void handle_line(char *line) {
  int j = 0;
  char formatted[1024];
  for (int i = 0; line[i]; i++) {
    if (line[i] == '/') {
      break;
    }
    if (line[i] == ' ' && j == 0)
      continue;
    if (!is_important(line[i])) {
      continue;
    }
    line[j++] = line[i];
  }
  line[j] = '\0';
}

static void parse_token(Lexer *lexer, char *word) { printf("%s\n", word); }

/*
 * This function must be parse a row and it can generate an expression
 */
static void parse_row(Lexer *lexer, char *line) {
  char c[1024];
  int j = 0;
  for (int i = 0; line[i]; i++) {
    if (!isalnum(line[i])) {
      if (j > 0) {
        c[j] = '\0';
        parse_token(lexer, c);
        j = 0;
      }
      continue;
    }
    c[j++] = line[i];
    // if (line[i] == '/')
    //   return;
    // if (!isalnum(line[i])) {
    //   if (j > 0) {
    //     c[j] = '\0';
    //     parse_token(lexer, c);
    //   }
    //   continue;
    // }
    // c[j++] = line[i];
  }
  printf("\nEnd of the line\n\n");
}

static void read_file(const char *filename) {
  FILE *fd = fopen(filename, "r");
  char line[1024];
  while (fgets(line, sizeof(line), fd)) {
    printf("%s", line);
  }
  printf("---------------\n\n");
}

Lexer *tokenize(const char *filename) {
  read_file(filename);
  FILE *fd = fopen(filename, "r");
  Lexer *lexer = malloc(sizeof(Lexer));
  char line[1024];
  while (fgets(line, 1024, fd)) {
    handle_line(line);
    if (line[0] == '\0')
      continue;
    parse_row(lexer, line);
  }

  return lexer;
}
