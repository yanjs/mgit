#ifndef MGIT_DB_H
#define MGIT_DB_H

#include <openssl/sha.h>
#include <stdio.h>

#include "types.h"
#include "utils.h"

#define MGIT_FOLDER ".mgit/"
#define MGIT_OBJECTS_FOLDER MGIT_FOLDER "objects/"
#define MGIT_DIRS_FOLDER MGIT_FOLDER "dirs/"

#define HASH_CTX SHA256_CTX
#define HASH_INIT SHA256_Init
#define HASH_UPDATE SHA256_Update
#define HASH_FINAL SHA256_Final

int db_store(OUT unsigned char hash[HASH_SIZE],
             IN const char* file_to_be_stored);
// caller provides a tmpfile
int db_fetch(OUT FILE** tmpfile,
             IN const char hash_of_file_to_be_fetched[HASH_STR_SIZE]);
int db_init(IN const char* mgit_dir_to_be_initialized);
int db_load();

extern char g_mgit_dir[MAX_FILENAME_SIZE];
extern char g_mgit_objects_dir[MAX_FILENAME_SIZE];

#endif