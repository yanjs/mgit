#include "objects.h"

static int print_file(FILE* fp) {
  char buff[BUFSIZ];
  size_t bytes_read;
  while ((bytes_read = fread(buff, sizeof(char), BUFSIZ, fp)) > 0) {
    for (size_t i = 0; i < bytes_read; ++i) {
      putc(buff[i], stdout);
    }
  }
  return MGIT_SUCCESS;
}

int cmd_cat_file(int argc, const char* argv[]) {
  for (int i = 1; i < argc; ++i) {
    cat_object(argv[i], print_file);
  }
  return MGIT_SUCCESS;
}
int cmd_hash_object(int argc, const char* argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Too few arguments for mgit hash_object!\n");
    return MGIT_ARGUMENT_ERROR;
  }
  for (int i = 1; i < argc; ++i) {
    hash_object(argv[i]);
  }
  return MGIT_SUCCESS;
}

int cmd_init(int argc, const char* argv[]) {
  const char* path = ".";
  if (argc >= 2) {
    path = argv[1];
  }
  if (init_mgit_dir(path) != 0) {
    fprintf(stderr, "Failed to initialize empty mgit repository at %s\n", path);
    return MGIT_FILE_WRITE_ERROR;
  } else {
    printf("Initialized empty mgit repository in %s\n", path);
    return MGIT_SUCCESS;
  }
}

int cmd_write_tree(int argc, const char* argv[]) {
  // TODO:
  return MGIT_SUCCESS;
}
