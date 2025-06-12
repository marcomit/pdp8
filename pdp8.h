#ifndef PDP8_H
#define PDP8_H

#include <stdint.h>

typedef struct pdp8_instr {
  uint8_t I;
  uint8_t OPR;
  uint16_t ADDRESS;
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

  uint16_t memory[4096];
  pdp8_instr *instructions;
} pdp8_emul;

pdp8_emul *pdp8_emul_new();
void free_pdp8_emul(pdp8_emul *);
void pdp8_tokenize(pdp8_emul *, const char *);

#endif // !PDP8_H
