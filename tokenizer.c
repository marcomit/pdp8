#include "tokenizer.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static bool is_important(char c) { return isalnum(c) || c == '-'; }

static Token *newtoken(char *word, int row, int col) {
  Token *token = malloc(sizeof(Token));

  token->col = col;
  token->row = row;

  token->val = malloc(sizeof(word));
  strcpy(token->val, word);

  char *types[20] = {"ADD", "AND", "LDA", "STA", "BUN",   "ISZ",       "CLA",
                     "CLE", "CMA", "CME", "CIR", "CIL",   "INC",       "SPA",
                     "SNA", "SZA", "SZE", "HLT", "COMMA", "IDENTIFIER"};

  for (int i = 0; i < 20; i++) {
    if (strcmp(word, types[i])) {
      token->type = (TokenType)i;
      break;
    }
  }

  return token;
}

static void add_token(Lexer *lexer, Token *token) {
  if (!lexer->head) {
    lexer->head = token;
  }
  if (!lexer->tail) {
    lexer->tail = token;
  }
  lexer->tail->next = token;
  lexer->tail = lexer->tail->next;
}

static void parse_token(Lexer *lexer, char *word, int row, int col) {
  Token *tk = newtoken(word, row, col);
  add_token(lexer, tk);
  // ciao
}

/*
 * this function must be parse a row and it can generate an expression
 */
static void parse_row(Lexer *lexer, char *line, int row) {
  char c[1024];
  int j = 0;
  for (int i = 0; line[i]; i++) {
    if (line[i] == '/')
      break;
    if (line[i] == ',') {
      c[j] = '\0';
      parse_token(lexer, ",", row, i);
      j = 0;
      continue;
    }
    if (!is_important(line[i])) {
      if (j > 0) {
        c[j] = '\0';
        parse_token(lexer, c, row, i);
        j = 0;
      }
      continue;
    }
    c[j++] = line[i];
  }
  parse_token(lexer, "\n", row, 0);
  // printf("----\n");
}

static void read_file(const char *filename) {
  FILE *fd = fopen(filename, "r");
  char line[1024];
  while (fgets(line, sizeof(line), fd)) {
    printf("%s", line);
  }
  printf("---------------\n\n");
}

static void print_tokens(Lexer *lx) {
  for (Token *curr = lx->head; curr; curr = curr->next) {
    printf("(%zu, %zu) %s\n", curr->row, curr->col, curr->val);
  }
}

Lexer *tokenize(const char *filename) {
  read_file(filename);
  FILE *fd = fopen(filename, "r");
  Lexer *lexer = malloc(sizeof(Lexer));
  char line[1024];
  int row = 0;
  while (fgets(line, 1024, fd)) {
    if (line[0] == '\0')
      continue;
    parse_row(lexer, line, row++);
  }

  print_tokens(lexer);
  return lexer;
}
