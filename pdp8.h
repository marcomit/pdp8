#ifndef PDP8_H
#define PDP8_H

#include "tokenizer.h"
#include <stdint.h>

/*
 * pdp8_instr is a struct for an instruction of PDP8.
 * In PDP8 the instruction has 16 bit.
 * I: 1 bit (indirect or not)
 * OPR: 3 bit.
 * ADDRESS: if OPR is 111 this is the operation else it represent the address of
 * memory
 */
typedef struct pdp8_instr {
  uint8_t I;
  uint8_t OPR;
  uint16_t ADDRESS;

  struct pdp8_instr *next;
} pdp8_instr;

typedef struct pdp8_emul {
  uint8_t I;

  uint8_t E;
  uint8_t R;
  uint8_t F;
  uint8_t S;

  uint16_t AC;
  uint16_t PC;
  uint16_t IR;
  uint16_t MA;
  uint16_t MB;
  uint16_t MQ;

  /* Location counter
   * It indicates the current memory position
   */
  uint16_t LC;

  uint16_t memory[4096];

  pdp8_instr *instructions;
  pdp8_instr *tail;
} pdp8_emul;

pdp8_emul *pdp8_emul_new();
void free_pdp8_emul(pdp8_emul *);
void pdp8_tokenize(pdp8_emul *, const char *);
void pdp8_get_oprs(pdp8_emul *, Lexer *);

#endif // !PDP8_H
