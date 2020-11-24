#ifndef MGIT_DB_H
#define MGIT_DB_H

#include <stdio.h>

#include "mgit_types.h"

#define MGIT_FOLDER ".mgit/"
#define MGIT_OBJECTS_FOLDER MGIT_FOLDER "objects/"
#define MGIT_DIRS_FOLDER MGIT_FOLDER "dirs/"

int db_store(unsigned char hash[MGIT_HASH_LENGTH],
             const char* file_to_be_stored);
FILE* db_fetch(const char* hash_of_file_to_be_fetched);

#endif