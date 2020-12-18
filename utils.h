#ifndef MGIT_UTILS_H
#define MGIT_UTILS_H

#include "types.h"

NORETURN void die();
NORETURN void die_prompt(const char* message);
const char* basename(IN const char* path);
int hash_to_str(OUT char str[HASH_STR_SIZE],
                IN const unsigned char hash[HASH_SIZE]);

#endif