#include "pdp8.h"
#include "tokenizer.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void usage() {
  printf("Usage:\n");
  printf("compile [filename]\n");
  printf("run [filename]\n");
}

static void compile(int argc, const char **argv) {
  if (argc < 4)
    return;
  const char *filename = argv[2];

  const char *target = argv[3];

  printf("Source: %s", filename);
  printf("Target: %s", target);

  pdp8_emul *emulator = pdp8_emul_new();

  printf("Tokenize\n");
  Lexer *lx = tokenize(filename);

  printf("Convert to oprs");
  pdp8_get_oprs(emulator, lx);
}

static void run(int argc, const char **argv) {}

int main(int argc, const char **argv) {
  if (argc < 3) {
    usage();
    return 1;
  }

  const char *filename = argv[2];

  if (strcmp(argv[1], "compile") == 0) {
    compile(argc, argv);
  } else if (strcmp(argv[1], "run") == 0) {
    run(argc, argv);
  } else {
    printf("Invalid command!\n");
    usage();
  }

  return 0;
}
