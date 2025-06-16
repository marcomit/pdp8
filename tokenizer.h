#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <stddef.h>
typedef enum TokenType {
  // Memory Reference Instructions   (MRI)
  AND, // And logico
  ADD, // Aggiunge l'indirizzo di memoria ad AC
  LDA,
  STA,
  BUN,
  BSA,
  ISZ,

  // Register Reference Instructions (RRI)
  CLA,
  CLE,
  CMA,
  CME,
  CIR,
  CIL,
  INC,
  SPA,
  SNA,
  SZA,
  SZE,
  HLT,

  COMMA,
  IDENTIFIER
} TokenType;

typedef struct Token {
  enum TokenType type;
  size_t col;
  size_t row;
  char *val;
  struct Token *next;
} Token;

typedef struct Lexer {
  Token *head;
  Token *tail;
  size_t len;
} Lexer;

Lexer *tokenize(const char *);

#endif // !TOKENIZER_H
