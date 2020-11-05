#ifndef MGIT_PATH_H
#define MGIT_PATH_H

#include "mgit_types.h"

#define MGIT_PATH_INIT_CAPACITY MGIT_BUFSIZ

typedef struct mgit_path {
  char *value;
  size_t len;
  size_t capacity;
} mgit_path_t;

#define MGIT_DIR ".mgit/"
#define MGIT_DIR_OBJECTS MGIT_DIR "objects/"
#define MGIT_DIR_DIRS MGIT_DIR "dirs/"

static const mgit_path_t s_mgit_dir = {
    .value = MGIT_DIR,
    .len = sizeof(MGIT_DIR) - 1,
    .capacity = 0,
};

static const mgit_path_t s_mgit_dir_objects = {
    .value = MGIT_DIR_OBJECTS,
    .len = sizeof(MGIT_DIR_OBJECTS) - 1,
    .capacity = 0,
};

static const mgit_path_t s_mgit_dir_dirs = {
    .value = MGIT_DIR_DIRS,
    .len = sizeof(MGIT_DIR_DIRS) - 1,
    .capacity = 0,
};

mgit_path_t *mgit_path_new(const char *path_str);
int mgit_path_resize(mgit_path_t *path, size_t capacity);

const char *mgit_path_read(const mgit_path_t *path);
int mgit_path_write(mgit_path_t *dest, const char *src);
int mgit_path_append(mgit_path_t *dest, const char *src);

int mgit_path_del(mgit_path_t *path);

#endif  // MGIT_PATH_H