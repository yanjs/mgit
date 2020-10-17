#ifndef MGIT_FILESYSTEM_H
#define MGIT_FILESYSTEM_H

#include "types.h"

int fs_mkdir(const char* path);

typedef enum mgit_file_enum {
  MGIT_FILE,
  MGIT_DIRECTORY,
  MGIT_SYMLINK,
} mgit_file_t;

struct mgit_dirent {
  mgit_hash_t hash;
  mgit_file_t type;
};

#endif  // MGIT_FILESYSTEM_H
