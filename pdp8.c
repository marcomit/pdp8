#include "pdp8.h"
#include "tokenizer.h"
#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ADVANCE(x) (x) = (x)->next

static TokenType mri[] = {
    AND, ADD, LDA, STA, BUN, BSA, ISZ,
};
static TokenType rri[] = {
    CLA, CLE, CMA, CME, CIR, CIL, INC, SPA, SNA, SZA, SZE, HLT,
};
static TokenType pseudo[] = {ORG, END, DEC, HEX, I, COMMA, NEW_LINE, LABEL};
static TokenType io[] = {INP, OUT, SKI, SKO, ION, IOF};

pdp8_emul *pdp8_emul_new() {
  pdp8_emul *emulator = malloc(sizeof(pdp8_emul));
  emulator->LC = 0;
  return emulator;
}

void free_pdp8_emul(pdp8_emul *emulator) {
  free(emulator->instructions);
  free(emulator);
}

static void bitf(size_t num, int len) {
  uint8_t bits[len];
  for (int i = 0; i < len; i++) {
    // printf("%zu", num & 1);
    bits[len - 1 - i] = num & 1;
    num >>= 1;
  }
  for (int i = 0; i < len; i++) {
    printf("%d", bits[i]);
  }
}

// It takes an 8 bit number but it convert only the 4 bits
static char bits_to_hex(uint8_t num) {
  num %= 16;
  char base = num < 10 ? '0' : 'A' - 10;
  return base + num;
}

static char *instr_to_hex(pdp8_instr *instr) {
  char *res = malloc(sizeof(char) * 7);
  // The idea is to take 4 bit and convert it to a single character
  // So now i want to decode an operation like this:
  // 3 hex char for location memory
  // 1 hex char for (I + OPR)
  // 3 hex char for ADDRESS of the operation
  for (int i = 0; i < 3; i++) {
    res[i] = bits_to_hex(instr->location >> ((2 - i) * 4));
  }

  uint8_t tmp = (instr->I << 3) + instr->OPR;
  res[3] = bits_to_hex(tmp);
  for (int i = 0; i < 3; i++) {
    res[4 + i] = bits_to_hex(instr->ADDRESS >> ((2 - i) * 4));
  }
  return res;
}

static bool hexstr_tohex(char *hex, size_t *res) {
  char *endptr = NULL;
  errno = 0;
  size_t value = strtol(hex, &endptr, 16);

  if (errno == ERANGE) {
    printf("Error: value out of range\n");
    return false;
  } else if (endptr == hex) {
    printf("Error: no digits were found\n");
    return false;
  } else if (*endptr != '\0') {
    printf("Error: extra characters after number: %s\n", endptr);
    return false;
  }
  *res = value;
  return true;
}

static void parse_operation(uint16_t val, pdp8_instr *instr) {
  instr->ADDRESS = val % 0x1000;
  instr->I = val >> 15;
  instr->OPR = (val << 1) >> 13;
}

static uint16_t from_instruction_to_bit(pdp8_instr *instr) {
  uint16_t res = 0;
  res = instr->I << 15;
  res += instr->OPR << 12;
  res += instr->ADDRESS;
  return res;
}

static void print_instructions(pdp8_instr *head) {
  pdp8_instr *c = head;
  uint16_t val;
  for (; c; ADVANCE(c)) {
    printf("%d ", c->location);
    val = from_instruction_to_bit(c);
    // while (val) {
    //   printf("%d", val & 1);
    //   val >>= 1;
    // }
    printf("%d", val);
    printf("\n");
  }
}

static uint16_t pdp8_get_label(pdp8_emul *emul, char *label) {
  pdp8_label_registry *h = emul->registry;
  for (; h; ADVANCE(h)) {
    if (strcmp(h->name, label) == 0) {
      return h->location;
    }
  }
  return -1;
}

static void pdp8_register_label(pdp8_emul *emul, char *label) {
  uint16_t index = -1;
  if (pdp8_get_label(emul, label) == index) {
  }

  pdp8_label_registry *entry = malloc(sizeof(pdp8_label_registry));

  entry->name = label;
  entry->location = emul->LC;
  if (!emul->registry) {
    emul->registry = entry;
    return;
  }
  pdp8_label_registry *head = emul->registry;
  while (head->next) {
    ADVANCE(head);
  }
  head->next = entry;
}

static void print_opr(pdp8_instr *instr) {

  printf("%d ", instr->location);
  bitf(instr->I, 1);
  bitf(instr->OPR, 3);
  bitf(instr->ADDRESS, 12);
  printf("\n");
}

static void add_opr(pdp8_emul *emul, pdp8_instr *instruction) {
  print_opr(instruction);
  if (!emul->instructions) {
    emul->instructions = instruction;
  }
  if (!emul->tail) {
    emul->tail = instruction;
  } else {
    emul->tail->next = instruction;
    ADVANCE(emul->tail);
  }
  emul->LC++;
}

static bool pdp8_check_token_type(TokenType *instructions, TokenType tt,
                                  int len) {
  for (int i = 0; i < len; i++) {
    if (instructions[i] == tt)
      return true;
  }
  return false;
}

static bool expected(Token *t, TokenType *permitted, int len) {
  bool res = pdp8_check_token_type(permitted, t->type, len);
  if (!res) {
    printf("Unexpected token %s at %zu:%zu\n", t->val, t->row, t->col);
    exit(1);
  }
  return res;
}

static pdp8_instr *pdp8_new_instr(pdp8_emul *emul) {
  pdp8_instr *instr = malloc(sizeof(pdp8_instr));
  instr->location = emul->LC;
  instr->ADDRESS = 0;
  instr->OPR = 0;
  instr->I = 0;
  return instr;
}

static void pdp8_free_instr(pdp8_instr *instr) {
  if (!instr)
    return;
  free(instr);
  pdp8_free_instr(instr->next);
}

static void parse_mri_instruction(pdp8_emul *emul, Token *line, int len) {
  if (len < 2) {
    printf("Invalid usage of Memory Referenced Instructions\n");
    printf("Error at %zu:%zu\n", line->row, line->row);
    exit(1);
  }
  pdp8_instr *instr = pdp8_new_instr(emul);
  instr->OPR = line->type % 8;
  ADVANCE(line);
  TokenType permitted[] = {LABEL};
  if (!expected(line, permitted, 1)) {
    goto clear;
  }

  // The first pass it try to get the label. If it is not found it must be set
  // in the second pass
  instr->ADDRESS = pdp8_get_label(emul, line->val);

  ADVANCE(line);

  if (len > 2) {
    permitted[0] = I;
    if (!expected(line, permitted, 1)) {
      goto clear;
    }
    instr->I = 1;
  }
  goto add;
clear:
  pdp8_free_instr(instr);
  goto end;
add:
  add_opr(emul, instr);
end:
  return;
}

static void parse_rri_operation(pdp8_emul *emul, Token *line, int len) {
  if (len > 1) {
    printf("Invalid usage of Register Referenced Instructions\n");
    printf("Unexpected token %s\n", line->next->val);
    printf("Error at %zu:%zu\n", line->row, line->col);
    exit(1);
  }

  pdp8_instr *instr = pdp8_new_instr(emul);
  instr->I = 0;
  instr->OPR = 7;
  instr->ADDRESS = 0x800;
  TokenType *loc_rri = rri;
  for (; loc_rri; (*loc_rri)++) {
    if (*loc_rri == line->type) {
      break;
    }
    instr->ADDRESS >>= 1;
  }
  add_opr(emul, instr);
}

static void parse_io_operation(pdp8_emul *emul, Token *line, int len) {
  printf("I/O Operation not implemented\n");
}

static void parse_pseudo_operation(pdp8_emul *emul, Token *line, int len) {
  if (!line)
    return;
  pdp8_instr *instr = pdp8_new_instr(emul);
  size_t val = 0;
  switch (line->type) {
  case ORG:
    if (!line->next) {
      printf("Missing memory target %zu:%zu\n", line->row, line->col);
      exit(1);
    }
    if (!hexstr_tohex(line->next->val, &val)) {
      printf("Invalid memory target %zu:%zu\n", line->row, line->col);
      exit(1);
    }
    emul->LC = val;
    break;
  case END:
    exit(1);
    break;
  case LABEL:
    if (len < 3) {
      printf("Missing argument of label at %zu:%zu\n", line->row, line->col);
      exit(1);
    }
    printf("Label detected %s\n", line->val);
    char *label = line->val;
    ADVANCE(line);
    TokenType permitted[] = {COMMA, DEC};
    if (!expected(line, permitted, 1)) {
      exit(1);
    }
    ADVANCE(line);
    permitted[0] = HEX;
    if (!expected(line, permitted, 2)) {
      exit(1);
    }
    pdp8_register_label(emul, label);
    ADVANCE(line);
    val = 0;
    if (line->type == DEC) {
      val = atoi(line->val);
    } else {
      bool res = hexstr_tohex(line->val, &val);
      if (!res) {
        printf("Error trying to convert the hex number at %zu:%zu\n", line->row,
               line->col);
        exit(1);
      }
    }
    parse_operation(val, instr);
    add_opr(emul, instr);
    break;
  default:
    printf("Pseudo operation %s not implemented\n", line->val);
    break;
  }
}

static void pdp8_print_tokens(Lexer *lx) {
  for (Token *c = lx->head; c; c = c->next) {
    printf("%s", c->val);
  }
}

static void token_to_operation(pdp8_emul *emul, Token *head, int len) {
  if (head == NULL) {
    printf("Parsing error!\n");
    exit(1);
  }
  TokenType *tokens[] = {mri, rri, pseudo, io};
  int lens[] = {7, 12, 8, 6};

  void (*parse[])(pdp8_emul *, Token *,
                  int) = {parse_mri_instruction, parse_rri_operation,
                          parse_pseudo_operation, parse_io_operation};

  char *labels[] = {"MRI", "RRI", "PSEUDO", "IO"};

  for (int i = 0; i < 4; i++) {
    if (pdp8_check_token_type(tokens[i], head->type, lens[i])) {
      printf("Decoded: %s | %s\n", labels[i], head->val);
      getchar();
      parse[i](emul, head, len);
      break;
    }
  }
}

static void print_registry(pdp8_emul *emul) {
  for (pdp8_label_registry *r = emul->registry; r; ADVANCE(r)) {
    printf("%d -> %s\n", r->location, r->name);
  }
}

static void pdp8_fetch_oprs(pdp8_emul *emul, Lexer *lx) {
  Token *head = NULL;
  Token *tail = NULL;
  int len = 0;

  // pdp8_print_tokens(lx);

  for (Token *c = lx->head; c; c = c->next) {
    if (c->type == NEW_LINE) {
      if (len > 0 && head)
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
  printf("\n\n\n\n");
  print_instructions(emul->instructions);

  print_registry(emul);
}

void pdp8_get_oprs(pdp8_emul *emul, Lexer *lx) {
  pdp8_print_tokens(lx);
  pdp8_fetch_oprs(emul, lx);

  for (pdp8_instr *c = emul->instructions; c; ADVANCE(c)) {
    pdp8_free_instr(c);
  }

  printf("Free the instructions\n");
  pdp8_fetch_oprs(emul, lx);
  printf("Finished\n\n");
  pdp8_instr *i = emul->instructions;
  while (i) {
    print_opr(i);
    ADVANCE(i);
  }
}

void pdp8_save_binary(pdp8_emul *emul, const char *filename) {
  FILE *fd = fopen(filename, "w");
  if (!fd) {
    printf("Error writing output file");
    exit(1);
  }
  pdp8_instr *i = emul->instructions;
  while (i) {
    char *opr = instr_to_hex(i);
    fprintf(fd, "%s\n", opr);
    ADVANCE(i);
  }
  fclose(fd);
}

void pdp8_fetch(pdp8_emul *emul) {
  emul->MAR = emul->PC;
  emul->MBR = emul->memory[emul->PC];
  emul->PC++;
  uint8_t i = emul->MBR >> 15 & 1;
  uint8_t opr = emul->MBR << 1;
  opr = opr >> 12;
  if (i && opr != 7) {
    // I
    emul->R = 1;
  } else {
    emul->F = 1;
  }
}

void pdp8_iaddressing(pdp8_emul *emul) {
  emul->MAR = emul->MBR << 4;
  emul->MAR >>= 4;
  emul->MBR = emul->memory[emul->PC];

  emul->F = 1;
  emul->R = 0;
}

void pdp8_execute(pdp8_emul *emul) {}

void pdp8_interrupt(pdp8_emul *emul) {}

void cycle(pdp8_emul *emul) {
  // 2 bit
  uint8_t decoder = ((emul->F & 1) << 1) + (emul->R & 1);
  void (*decodes[])(pdp8_emul *) = {pdp8_fetch, pdp8_iaddressing, pdp8_execute,
                                    pdp8_interrupt};
  decodes[decoder](emul);
}
