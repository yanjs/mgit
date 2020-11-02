#ifndef MGIT_PATH_H
#define MGIT_PATH_H

#include "mgit_types.h"

#define MGIT_PATH_INIT_CAPACITY MGIT_BUFSIZ

typedef struct mgit_path {
  char *value;
  size_t len;
  size_t capacity;
} mgit_path_t;

mgit_path_t *mgit_path_new(const char *path_str);
int mgit_path_resize(mgit_path_t *path, size_t capacity);

const char *mgit_path_read(const mgit_path_t *path);
int mgit_path_write(mgit_path_t *dest, const char *src);
int mgit_path_append(mgit_path_t *dest, const char *src);

int mgit_path_del(mgit_path_t *path);

#endif  // MGIT_PATH_H