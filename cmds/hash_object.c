#include "hash_object.h"

#include "../db.h"

static int hash_object(const char* filename) {
  unsigned char hash[HASH_SIZE];
  if (!db_store(hash, filename)) return READ_FAILURE;
  return SUCCESS;
}

int cmd_hash_object(int argc, const char* argv[]) {
  for (int i = 1; i < argc; i++) {
    hash_object(argv[i]);
  }
  return SUCCESS;
}