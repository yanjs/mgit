#ifndef _TYPES_H
#define _TYPES_H

#define MGIT_SUCCESS 0
#define MGIT_FILE_OPEN_ERROR 2

#include <stdio.h>

typedef int cmd_handler(int argc, const char* argv[]);
typedef int file_handler(FILE* fp);

struct cmd {
    const char* cmd_name;
    cmd_handler* handler;
};

#endif
