
#include "experiments.h"

#include <stdio.h>

#include "mgit_types.h"
#include "objects.c"

int for_each_in_file(const char* path, size_t nfbs, ...) {
  FILE* f = fopen(path, "r");
  if (f == NULL) {
    return MGIT_FILE_OPEN_ERROR;
  }
  va_list vl;
  int result = 0;
  size_t read_len;
  unsigned char buff[BUFSIZ];
  struct mgit_file_block_handlers* curr_handlers;

  va_start(vl, nfbs);
  for (size_t i = 0; i < nfbs; ++i) {
    curr_handlers = va_arg(vl, struct mgit_file_block_handlers*);
    if (curr_handlers != NULL && curr_handlers->before != NULL) {
      if (curr_handlers->before() != MGIT_SUCCESS) {
        curr_handlers->handler = NULL;
        curr_handlers->after = NULL;
        result = -1;
      };
    }
  }

  while ((read_len = fread(buff, sizeof(char), BUFSIZ, f)) > 0) {
    va_start(vl, nfbs);
    for (size_t i = 0; i < nfbs; ++i) {
      curr_handlers = va_arg(vl, struct mgit_file_block_handlers*);
      if (curr_handlers != NULL && curr_handlers->handler != NULL) {
        if (curr_handlers->handler(read_len, buff) != MGIT_SUCCESS) {
          curr_handlers->handler = NULL;
          result = -1;
        }
      }
    }
  }

  va_start(vl, nfbs);
  for (size_t i = 0; i < nfbs; ++i) {
    curr_handlers = va_arg(vl, struct mgit_file_block_handlers*);
    if (curr_handlers != NULL && curr_handlers->after != NULL) {
      curr_handlers->after();
    }
  }

  va_end(vl);
  fclose(f);
  return result;
}
