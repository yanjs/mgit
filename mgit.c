#include "subcommands.h"
#include <string.h>
#include <stdio.h>

static struct cmd subcommands[] = {
    {"hash-object", cmd_hash_object},
    {"init", cmd_init},
};

static const size_t n_subcommands = (sizeof(subcommands) / sizeof (struct cmd));

int main(int argc, const char* argv[]) {
    if (argc <= 1) {
        return -1;
    }

    for (int i = 0; i < argc; i++) {
        printf("argv: %s\n", argv[i]);
    }
    for (size_t i = 0; i < n_subcommands; i++) {
        if (strcmp(subcommands[i].cmd_name, argv[1]) == 0) {
            subcommands[i].handler(argc - 1, &argv[1]);
            return 0;
        }
    }
}
