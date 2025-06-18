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
  if (len == 1) {
    return;
  }
}

void pdp8_get_oprs(pdp8_emul *emul, Lexer *lx) {
  int len = 0;

  Token *head;
  Token *tail;

  for (Token *c = lx->head; c; c = c->next) {
    if (c->type == NEW_LINE) {
      if (len > 0) {
        token_to_operation(emul, head, len);
      }
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
