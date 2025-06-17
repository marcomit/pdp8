#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <stddef.h>
typedef enum TokenType {
  // Memory Reference Instructions   (MRI)
  AND,
  ADD,
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

  // Input/Output Instructions
  INP,
  OUT,
  SKI,
  SKO,
  ION,
  IOF,

  // Pseudo instructions
  ORG,
  END,
  DEC,
  HEX,

  // Other tokens
  I,
  COMMA,
  NEW_LINE,
  LABEL
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
