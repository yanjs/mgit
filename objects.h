#ifndef MGIT_OBJECTS_H
#define MGIT_OBJECTS_H

#include "mgit_types.h"

#define MGIT_FOLDER ".mgit/"
#define MGIT_OBJECTS_FOLDER MGIT_FOLDER "objects/"
#define MGIT_DIRS_FOLDER MGIT_FOLDER "dirs/"

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

int init_mgit_dir();

int is_ignored(const char* path);

int hash_to_hex(mgit_hash_string_t* hash_hex, mgit_hash_t* hash);
int hash_object(mgit_hash_t* hash, const char* path);

int get_file_hash(mgit_hash_t* hash, const char* path);

int cat_object(const char* path, file_handler* handler);

int fs_listdir(const char* path);

#endif  // MGIT_OBJECTS_H
