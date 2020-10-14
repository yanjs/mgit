#include "cmd_hash_object.h"
#include "objects.h"
#include "types.h"
#include <stdio.h>

int cmd_hash_object(int argc, const char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Too few arguments for mgit hash_object!\n");
        return MGIT_ARGUMENT_ERROR;
    }
    for (int i = 1; i < argc; ++i) {
        hash_object(argv[i]);
    }
    return MGIT_SUCCESS;
}

