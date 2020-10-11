#ifndef _TYPES_H
#define _TYPES_H

typedef int cmd_handler(int argc, const char* argv[]);

struct cmd {
    const char* cmd_name;
    cmd_handler* handler;
};

#endif
