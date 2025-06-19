#include "pdp8.h"
#include "tokenizer.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

pdp8_emul *pdp8_emul_new() {
  pdp8_emul *emulator = malloc(sizeof(pdp8_emul));
  return emulator;
}

void free_pdp8_emul(pdp8_emul *emulator) {
  free(emulator->instructions);
  free(emulator);
}

static void add_opr(pdp8_emul *emul, pdp8_instr *instruction) {
  if (!emul->instructions) {
    emul->instructions = instruction;
    emul->TMP = 1;
  }
  if (!emul->tail) {
    emul->tail = instruction;
    emul->TMP = 1;
  } else {
    emul->tail->next = instruction;
    emul->tail = emul->tail->next;
    emul->TMP++;
  }
}

static bool pdp8_check_toekn_type(TokenType *instructions, TokenType tt) {
  for (; instructions; (*instructions)++) {
    if (*instructions == tt)
      return true;
  }
  return false;
}

static bool is_mri_operation(Token *tk) {
  TokenType mri[] = {AND, ADD, LDA, STA, BUN, BSA, ISZ};
  return pdp8_check_toekn_type(mri, tk->type);
}

static bool is_rri_operation(Token *tk) {
  TokenType rri[] = {
      CLA, CLE, CMA, CME, CIR, CIL, INC, SPA, SNA, SZA, SZE, HLT,
  };
  return pdp8_check_toekn_type(rri, tk->type);
}

static bool is_IO_operation(Token *tk) {
  TokenType io[] = {INP, OUT, SKI, SKO, ION, IOF};
  return pdp8_check_toekn_type(io, tk->type);
}

static bool is_implicit_operation(Token *tk) {
  TokenType impl[] = {ORG, END, DEC, HEX, I, COMMA, NEW_LINE, LABEL};
  return pdp8_check_toekn_type(impl, tk->type);
}

static void token_to_operation(pdp8_emul *emul, Token *head, int len) {
  pdp8_instr *instr = malloc(sizeof(pdp8_instr));
}

static void parse_mri_instruction(pdp8_emul *emul, Token *line, int len) {}

static void parse_rri_operation(pdp8_emul *emul, Token *line, int len) {
  if (len > 1) {
    printf("Invalid usage of Register Referenced Instructions\n");
    printf("Error at %zu:%zu\n", line->row, line->col);
    return;
  }
}

static void parse_io_operation(pdp8_emul *emul, Token *line, int len) {}

static void parse_label_operation(pdp8_emul *emul, Token *line, int len) {}

static void pdp8_print_tokens(Lexer *lx) {
  // for (Token *c = lx->head; c; c = c->next) {
  //   printf();
  // }
}

void pdp8_get_oprs(pdp8_emul *emul, Lexer *lx) {
  Token *head;
  Token *tail;
  int len = 0;

  for (Token *c = lx->head; c; c = c->next) {
    if (c->type == NEW_LINE) {
      token_to_operation(emul, head, len);
      head = NULL;
      tail = NULL;
      continue;
    }
    if (!head) {
      head = c;
      len = 1;
    }
    if (!tail) {
      tail = c;
      len = 1;
    } else {
      tail->next = c;
      tail = c;
      len++;
    }
  }
}
