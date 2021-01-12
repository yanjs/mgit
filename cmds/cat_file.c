#include "cat_file.h"

#include "../db.h"

static int cat_file(const char* hash) {
  FILE* f = tmpfile();
  db_fetch(&f, hash);
  unsigned char buffer[FILE_READ_BYTES];
  size_t nread;
  rewind(f);
  while (1) {
    nread = fread(buffer, 1, FILE_READ_BYTES, f);
    if (nread == 0) {
      break;
    }
    for (size_t i = 0; i < nread; i++) {
      putc(buffer[i], stdout);
    }
  }
  return SUCCESS;
}

int cmd_cat_file(int argc, const char* argv[]) {
  for (int i = 1; i < argc; i++) {
    cat_file(argv[i]);
  }
  return SUCCESS;
}