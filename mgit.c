#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cmds/hash_object.h"
#include "cmds/init.h"

static struct mgit_cmd subcommands[] = {
    //{"cat-file", cmd_cat_file},
    {"hash-object", cmd_hash_object, CMD_LOAD_DB},
    {"init", cmd_init, CMD_NORMAL},
    //{"write-tree", cmd_write_tree},
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
  if (argc == 1) {
    puts("mgit usage:");
    exit(0);
  }
#ifdef MGIT_TEST_SUBCOMMANDS
  for (int i = 0; i < argc; i++) {
    printf("argv[%i]: %s\n", i, argv[i]);
  }
#endif
  for (size_t i = 0; i < n_subcommands; i++) {
    if (strcmp(subcommands[i].name, argv[1]) == 0) {
      return subcommands[i].handler(argc - 1, &argv[1]);
    }
  }
}
