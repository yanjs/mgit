#ifndef MGIT_PATH_H
#define MGIT_PATH_H

#include "mgit_types.h"

#define MGIT_PATH_LENGTH MGIT_BUFSIZ

typedef struct mgit_path {
  char value[MGIT_PATH_LENGTH + 1];
  char *cursor;
} mgit_path_t;

mgit_path_t *mgit_path_new(const char *path);

char *mgit_path_read(mgit_path_t *path);

mgit_path_t *mgit_path_write(const char *path);

mgit_path_t *mgit_path_cat(mgit_path_t *dest, mgit_path_t *src);

int mgit_path_del(mgit_path_t *path);

#endif  // MGIT_PATH_H