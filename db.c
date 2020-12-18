#include "db.h"

#include <string.h>

#include "fs.h"
#include "utils.h"
// Every directory must ends with a '/'
char g_mgit_dir[MAX_FILENAME_SIZE] = ".mgit/";
char g_mgit_objects_dir[MAX_FILENAME_SIZE] = ".mgit/objects/";

static int store(IN const char* hashstr, IN const char* filename) {
  // to do in the future: handle path with more characters
  char leader[4];
  leader[0] = hashstr[0];
  leader[1] = hashstr[1];
  leader[2] = '/';
  leader[3] = '\0';

  // to do in the future: fix this part for a very long path
  char objname[MAX_FILENAME_SIZE];
  strncpy(objname, g_mgit_objects_dir, MAX_FILENAME_SIZE);
  strcat(objname, leader);
  fs_mkdir(objname);
  strcat(objname, &hashstr[2]);

  FILE* f = fopen(objname, "w");
  FILE* srcf = fopen(filename, "r");
  unsigned char buffer[FILE_READ_BYTES];
  size_t nread;

  while (1) {
    nread = fread(buffer, 1, FILE_READ_BYTES, srcf);
    if (nread == 0) {
      break;
    }
    if (fwrite(buffer, nread, 1, f) != nread) {
      // clear bytes written
      freopen(objname, "w", f);
      die_prompt(objname);
    }
  }
  fclose(f);
  return SUCCESS;
}

static int get_hash(OUT unsigned char* hash, IN const char* path) {
  char buffer[FILE_READ_BYTES];
  size_t nread;
  FILE* f = fopen(path, "r");
  if (!f) die_prompt("path");

  HASH_CTX hashctx;
  HASH_INIT(&hashctx);

  while (1) {
    nread = fread(buffer, 1, FILE_READ_BYTES, f);
    printf("%ld\n", nread);
    // to do in the future: test if eof
    if (nread == 0) {
      break;
    }
    HASH_UPDATE(&hashctx, buffer, nread);
  }

  HASH_FINAL(hash, &hashctx);
  printf("%lx\n", *(long*)hash);
  fclose(f);
  return SUCCESS;
}

int db_store(OUT unsigned char hash[HASH_SIZE], IN const char* filename) {
  get_hash(hash, filename);
  char hash_str[HASH_STR_SIZE];
  hash_to_str(hash_str, hash);
  store(hash_str, filename);
  return SUCCESS;
}