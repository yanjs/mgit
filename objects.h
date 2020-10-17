#ifndef MGIT_OBJECTS_H
#define MGIT_OBJECTS_H

#include "mgit_types.h"

#define HASH_STRING_BYTES 65
#define MGIT_FOLDER ".mgit/"
#define MGIT_OBJECTS_FOLDER (MGIT_FOLDER "objects/")
#define MGIT_DIRS_FOLDER (MGIT_FOLDER "dirs/")

const char* mgit_directories[] = {
    MGIT_FOLDER,
    MGIT_OBJECTS_FOLDER,
    MGIT_DIRS_FOLDER,
};

int init_mgit_dir();

int hash_object(const char* path);

int get_file_hash(mgit_hash_t hash, const char* path);

int cat_object(const char* path, file_handler* handler);
#endif  // MGIT_OBJECTS_H
