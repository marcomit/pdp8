#include "pdp8.h"
#include "tokenizer.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, const char **argv) {
  if (argc < 2) {
    printf("Please insert the filename to execute");
  }
  const char *filename = argv[1];
  pdp8_emul *emulator = pdp8_emul_new();
  Lexer *lx = tokenize(filename);

  pdp8_get_oprs(emulator, lx);
  return 0;
}
