#include "pdp8.h"
#include "tokenizer.h"
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "std.h"

static void usage() {
  printf("Usage:\n");
  printf("compile [filename]\n");
  printf("run [filename]\n");
}

static void compile(int argc, const char **argv) {
  if (argc < 4) {
    printf("Output file is missing.\n");
    return;
  }
  const char *filename = argv[2];

  const char *target = argv[3];

  printf("Source: %s\n", filename);
  printf("Target: %s\n", target);

  pdp8_emul *emulator = pdp8_emul_new();

  Lexer *lx = tokenize(filename);

  printf("Convert to oprs\n");
  pdp8_get_oprs(emulator, lx);
  printf("Compiled\n");
  pdp8_save_binary(emulator, target);
}

static void run(int argc, const char **argv) {}

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
  num <<= 4;
  num >>= 4;
  char base = num < 10 ? '0' : 'A' - 10;
  return base + num;
}

int main(int argc, const char **argv) {

  if (argc < 3) {
    usage();
    return 1;
  }

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
