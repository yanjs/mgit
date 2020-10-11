#include "cmd_hash_object.h"
#include "objects.h"
#include <stdio.h>

int cmd_hash_object(int argc, const char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Too few arguments for mgit hash_object!\n");
        return -1;
    }
    for (int i = 1; i < argc; ++i) {
        hash_object(argv[i]);
    }
    return 0;
}

