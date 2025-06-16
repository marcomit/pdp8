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
  }
  if (!emul->tail) {
    emul->tail = instruction;
  } else {
    emul->tail->next = instruction;
    emul->tail = emul->tail->next;
  }
}

static void token_to_operation(pdp8_emul *emul, Token *head, int len) {
  TokenType mri[] = {AND, ADD, LDA, STA, BUN, BSA, ISZ};
  pdp8_instr *instr = malloc(sizeof(pdp8_instr));

  for (int i = 0; i < 7; i++) {
    if (head->type == mri[i]) {
      instr->OPR = mri[i];
      break;
    }
  }
  if (!head->next) {
    printf("Expected a memory address %zu:%zu\n", head->row, head->col);
    return;
  }
  head = head->next;
  if (head->type) {
    printf("Expected an identifier, found %d\n", head->type);
    return;
  }
  head = head->next;
  // if ()
}
void pdp8_get_oprs(pdp8_emul *emul, Lexer *lx) {
  Token *head;
  Token *tail;

  int len = 0;

  Token *dummy = lx->head;

  for (; dummy; dummy = dummy->next) {
    if (dummy->type == NEW_LINE) {
      // printf("End line %s\n", dummy->val);
      token_to_operation(emul, head, len);
      len = 0;
      head = NULL;
      tail = NULL;
      continue;
    }
    if (!head)
      head = dummy;
    if (!tail)
      tail = dummy;
    else {
      tail->next = dummy;
      tail = tail->next;
    }
    len++;
  }
}
