#include <stdio.h>
#include <string.h>

#include "subcommands.h"

static struct mgit_cmd subcommands[] = {
    {"cat-file", cmd_cat_file},
    {"hash-object", cmd_hash_object},
    {"init", cmd_init},
    {"write-tree", cmd_init},
};

static const size_t n_subcommands =
    (sizeof(subcommands) / sizeof(struct mgit_cmd));

int
#ifndef MGIT_TEST_SUBCOMMANDS
main
#else
test_main
#endif
(int argc, const char* argv[]) {
  if (argc <= 1) {
    return MGIT_ARGUMENT_ERROR;
  }
#ifdef MGIT_TEST_SUBCOMMANDS
  for (int i = 0; i < argc; i++) {
    printf("argv[%i]: %s\n", i, argv[i]);
  }
#endif
  for (size_t i = 0; i < n_subcommands; i++) {
    if (strcmp(subcommands[i].cmd_name, argv[1]) == 0) {
      return subcommands[i].handler(argc - 1, &argv[1]);
    }
  }
}
