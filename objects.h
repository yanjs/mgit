#ifndef MGIT_OBJECTS_H
#define MGIT_OBJECTS_H

#include "mgit_types.h"

#define MGIT_HASH_STRING_BYTES 65
#define MGIT_FOLDER ".mgit/"
#define MGIT_OBJECTS_FOLDER MGIT_FOLDER "objects/"
#define MGIT_DIRS_FOLDER MGIT_FOLDER "dirs/"

int init_mgit_dir();

int for_each_in_file(const char* path, size_t nfbs, ...);

int hash_object(const char* path);

int get_file_hash(mgit_hash_t hash, const char* path);

int cat_object(const char* path, file_handler* handler);
extern struct mgit_file_block_handlers g_mgit_fbs_hash;
const char* mgit_fbs_get_file_hash_hex();
const unsigned char* mgit_fbs_get_file_hash();
extern struct mgit_file_block_handlers g_mgit_fbs_copy;

#endif  // MGIT_OBJECTS_H
