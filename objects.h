#ifndef _OBJECTS_H
#define _OBJECTS_H

#define HASH_STRING_BYTES 65
#define MGIT_FOLDER ".mgit/"
#define MGIT_OBJECTS_FOLDER MGIT_FOLDER "objects/"

int init_mgit_dir(const char* path);

int hash_object(const char* path);

int sha_file(char dest_hash[HASH_STRING_BYTES], const char* path);
#endif // _OBJECTS_H
