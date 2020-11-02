#include "mgit_path.h"

#include <stdlib.h>
#include <string.h>

/**
 * Malloc a new mgit_path_t object
 */
mgit_path_t *mgit_path_new(const char *path_str) {
  mgit_path_t *p = malloc(sizeof(mgit_path_t));
  if (!p) {
    return NULL;
  }
  p->len = strlen(path_str);
  p->capacity = p->len + MGIT_HASH_STRING_BYTES;
  p->value = calloc(1, p->capacity * sizeof(char));
  if (!p->value) {
    free(p);
    return NULL;
  }

  strcpy(p->value, path_str);
  return p;
}

/**
 * Realloc a mgit_path_t object
 */
int mgit_path_resize(mgit_path_t *path, size_t capacity) {
  char *new_value = realloc(path->value, capacity);
  if (!new_value) {
    return MGIT_NOMEM_ERROR;
  }
  path->value = new_value;
  path->capacity = capacity;
  return MGIT_SUCCESS;
}

/**
 * Return the c_str of mgit_path_t object
 */
const char *mgit_path_read(const mgit_path_t *path) { return path->value; }

/**
 * Write src into mgit_path_t object dest
 */
int mgit_path_write(mgit_path_t *dest, const char *src) {
  size_t req_capacity = strlen(src) + 1;

  // capacity guarantee
  if (dest->capacity < req_capacity) {
    if (!mgit_path_resize(dest, req_capacity + 16)) {
      return MGIT_NOMEM_ERROR;
    }
  }

  dest->len = req_capacity - 1;
  strncpy(dest->value, src, dest->capacity);

  return MGIT_SUCCESS;
}

/**
 * Append src into mgit_path_t object dest. Add a slash if required
 */
int mgit_path_append(mgit_path_t *dest, const char *src) {
  int need_slash = dest->len > 0 && dest->value[dest->len - 1] != '/';
  size_t req_capacity = strlen(src) + dest->len + need_slash + 1;

  // capacity guarantee
  if (dest->capacity < req_capacity) {
    if (!mgit_path_resize(dest, req_capacity + 16)) {
      return MGIT_NOMEM_ERROR;
    }
  }

  // set slash
  if (need_slash) {
    dest->value[dest->len] = '/';
    dest->len++;
  }

  dest->len = req_capacity - 1;
  strcpy(&dest->value[dest->len], src);

  return MGIT_SUCCESS;
}

/**
 * Free a mgit_path_t object
 */
int mgit_path_del(mgit_path_t *path) {
  free(path->value);
  free(path);
  return MGIT_SUCCESS;
}