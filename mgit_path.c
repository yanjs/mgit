#include "mgit_path.h"

#include <stdlib.h>
#include <string.h>

#ifdef MGIT_DEBUG
static size_t s_mgit_path_count = 0;
#endif

/**
 * calloc a new mgit_path_t object
 */
mgit_path_t *mgit_path_new(const char *path_str) {
  mgit_path_t *p = calloc(1, sizeof(mgit_path_t));
  if (!p) {
    return NULL;
  }
  if (!mgit_path_init(p, path_str)) {
    free(p);
    return NULL;
  }

#ifdef MGIT_DEBUG
  s_mgit_path_count++;
  printf("new mgit_path object, mgit_path_count: %zd\n", s_mgit_path_count);
#endif

  return p;
}

/**
 * Initialize a path object. Return NULL if malloc failed.
 */
mgit_path_t *mgit_path_init(mgit_path_t *p, const char *path_str) {
  size_t len = strlen(path_str);
  size_t capacity = len + MGIT_HASH_STRING_BYTES;
  char *value = malloc(sizeof(char) * capacity);
  if (!value) {
    return NULL;
  }
  p->value = value;
  p->len = len;
  p->capacity = capacity;

  strcpy(p->value, path_str);

#ifdef MGIT_DEBUG
  s_mgit_path_count++;
  printf("new mgit_path object, mgit_path_count: %zd\n", s_mgit_path_count);
#endif

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
#ifdef MGIT_DEBUG
  s_mgit_path_count--;
  printf("del mgit_path object, mgit_path_count: %zd\n", s_mgit_path_count);
#endif
  return MGIT_SUCCESS;
}