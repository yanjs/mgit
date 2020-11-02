#include "mgit_path.h"

#include <stdlib.h>
#include <string.h>

/**
 * Malloc a new mgit_path_t object
 */
mgit_path_t *mgit_path_new(const char *path) {
  mgit_path_t *p = (mgit_path_t *)malloc(sizeof(mgit_path_t));
  strncpy(p, path, MGIT_PATH_LENGTH);
  p->value[MGIT_PATH_LENGTH] = '\0';
  p->cursor = p->value;
  return p;
}

mgit_path_t *mgit_path_cat(mgit_path_t *dest, mgit_path_t *src) { return NULL; }

int mgit_path_del(mgit_path_t *path) {
  free(path);
  return MGIT_SUCCESS;
}