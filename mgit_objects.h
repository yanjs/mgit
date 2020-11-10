#ifndef MGIT_OBJECTS_H
#define MGIT_OBJECTS_H

#include "mgit_path.h"
#include "mgit_types.h"

int mgit_dir_init();
int mgit_file_is_ignored(mgit_path_t fname);
int mgit_hex_from_hash(mgit_hash_string_t hash_hex, mgit_hash_t hash);
int mgit_hash_from_hex(mgit_hash_t hash, mgit_hash_string_t hash_hex);
int mgit_object_store(mgit_hash_t hash, mgit_path_t path);
int mgit_object_fetch(mgit_path_t path, mgit_hash_t hash);
int mgit_tree_store(mgit_hash_t hash, mgit_path_t path);
int mgit_tree_fetch(mgit_path_t path, mgit_hash_t hash);

#endif