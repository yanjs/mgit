#include <stdio.h>

int cmd_init(int argc, char* argv[]) {
    char* path = ".";
    if (argc >= 2) {
        path = argv[1];
    }
    if (init_mgit_dir(path) != 0) {
        fprintf(stderr, "Failed to initialize empty mgit repository at %s\n", path);
        return -1;
    } else {
        printf("Initialized empty mgit repository in %s\n", path);
        return 0;
    }
}
