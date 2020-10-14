#ifndef MGIT_OBJECTS_H
#define MGIT_OBJECTS_H

#include "types.h"

#define HASH_STRING_BYTES 65
#define MGIT_FOLDER ".mgit/"
#define MGIT_OBJECTS_FOLDER MGIT_FOLDER "objects/"

int init_mgit_dir(const char* path);

int hash_object(const char* path);

int get_file_hash(mgit_hash_t hash, const char* path);

int cat_object(const char* path, file_handler* handler);
#endif // MGIT_OBJECTS_H
