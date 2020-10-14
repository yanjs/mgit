#include "cmd_cat_file.h"
#include "objects.h"


static int print_file(FILE* fp) {
    char buff[BUFSIZ];
    size_t bytes_read;
    while ((bytes_read = fread(buff, sizeof(char), BUFSIZ, fp)) > 0) {
        for (size_t i = 0; i < bytes_read; ++i) {
            putc(buff[i], stdout);
        }
    }
    return MGIT_SUCCESS;
}

int cmd_cat_file(int argc, const char* argv[]) {
    for (int i = 1; i < argc; ++i) {
        cat_object(argv[i], print_file);
    }
    return MGIT_SUCCESS;
}
