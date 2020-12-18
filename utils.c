#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

NORETURN void die() { die_prompt("mgit"); }

NORETURN void die_prompt(IN const char* msg) {
  perror(msg);
  exit(EXIT_FAILURE);
}

int hash_to_str(OUT char str[HASH_STR_SIZE],
                IN const unsigned char hash[HASH_SIZE]) {
  for (size_t count = 0; count < HASH_SIZE; count++) {
    sprintf(&str[2 * count], "%02x", hash[count]);
  }
  return SUCCESS;
}

const char* basename(IN const char* path) {
  if (*path == '\0') {
    return path;
  }
  char* slash = strchr(path, '/');
  if (!slash) {
    return path;
  }

  const char* curr = path;
  const char* ret = path;

  while (1) {
    if (*curr == '/' && curr[1] != '/') {
      if (curr[1] == '\0') {
        return ret;
      } else {
        curr++;
        ret = curr;
      }
    }
  }
}