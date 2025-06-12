#include "pdp8.h"
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

static uint16_t evaluate_expr(char *c) {
  // Memory Referenced Instructions (MRI)
  char *mri[7] = {
      "AND", // 001
      "ADD", // 010
      "LDA", // 011
      "STA", // 100
      "BUN", // 101
      "BSA", // 110
      "ISZ"  // 111
  };
  for (int i = 0; i < 7; i++) {
    if (strcmp(mri[i], c)) {
      return i;
    }
  }

  return 0;
}

static int is_important(char c) { return isalnum(c) || c == ','; }

static char *skip_blankspaces(char *line, int row) {
  char opr[1024];
  uint16_t cnt = 0;

  for (char *ptr = line; *ptr; ptr++) {
    if (*ptr == '/') {
      break;
    }
    if (is_important(*ptr)) {
      opr[cnt++] = *ptr;
    }
  }

  char *op = malloc(sizeof(char) * (cnt + 1));
  opr[cnt] = '\0';
  return op;
}

void pdp8_tokenize(pdp8_emul *emulator, const char *filename) {
  FILE *fd = fopen(filename, "r");

  printf("%s\n", filename);

  char line[1024];
  int row = 0;
  while (fgets(line, sizeof(line), fd)) {
    // printf("%s", line);
    skip_blankspaces(line, row);
    row++;
  }
}
