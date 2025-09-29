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

  char *types[32] = {"AND", "ADD", "LDA", "STA", "BUN", "BSA", "ISZ", "CLA",
                     "CLE", "CMA", "CME", "CIR", "CIL", "INC", "SPA", "SNA",
                     "SZA", "SZE", "HLT", "INP", "OUT", "SKI", "SKO", "ION",
                     "IOF", "ORG", "END", "DEC", "HEX", "I",   ",",   "\n"};

  for (int i = 0; i < 32; i++) {
    if (strcmp(word, types[i]) == 0) {
      token->type = (TokenType)i;
      break;
    }
  }
  if (!token->type) {
    token->type = LABEL;
  }

  return token;
}

static void add_token(Lexer *lexer, Token *token) {
  if (!lexer->head) {
    lexer->head = token;
  }
  if (!lexer->tail) {
    lexer->tail = token;
  } else {
    lexer->tail->next = token;
    lexer->tail = token;
  }
}

static void parse_token(Lexer *lexer, char *word, int row, int col) {
  Token *tk = newtoken(word, row, col);
  add_token(lexer, tk);
}

static void cut_token(Lexer *lexer, int *i, char *line, int row, int col) {
  if (*i <= 0) {
    return;
  }
  line[*i] = '\0';
  parse_token(lexer, line, row, col);
  *i = 0;
}

static void parse_row(Lexer *lexer, char *line, int row) {
  char c[1024];
  int j = 0;
  for (int i = 0; line[i]; i++) {
    if (line[i] == '/')
      break;
    if (line[i] == ',') {
      cut_token(lexer, &j, c, row, i);
      c[j] = '\0';
      parse_token(lexer, ",", row, i);
      j = 0;
      continue;
    }
    if (!is_important(line[i])) {
      cut_token(lexer, &j, c, row, i);
      continue;
    }
    c[j++] = line[i];
  }
  parse_token(lexer, "\n", row, 0);
}

Lexer *tokenize(const char *filename) {
  FILE *fd = fopen(filename, "r");
  Lexer *lexer = malloc(sizeof(Lexer));
  char line[1024];
  int row = 0;
  while (fgets(line, 1024, fd)) {
    if (line[0] == '\0')
      continue;
    parse_row(lexer, line, row++);
  }
  return lexer;
}

void print_tokens(Lexer *l) {
  Token *token = l->head;

  if (!token) {
    printf("Tokens are empty\n");
    return;
  }

  while (token) {
    printf("%d: %s\n", token->type, token->val);
    token = token->next;
  }
}
