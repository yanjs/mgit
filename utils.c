#include "utils.h"

#include "stdlib.h"

void die() {
  perror("mgit");
  exit(EXIT_FAILURE);
}

const char* basename(const char* path) {
  if (*path == '\0') {
    return path;
  }
  char* slash = strchr(path, '/');
  if (!slash) {
    return path;
  }

  char* curr = path;
  char* ret = path;

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