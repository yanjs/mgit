#include "objects.h"

#include <openssl/sha.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "filesystem.h"

int init_mgit_dir(const char* path) {
  char pathbuf[BUFSIZ] = {0};
  int return_code;

  snprintf(pathbuf, BUFSIZ, "%s/%s", path, MGIT_FOLDER);
  return_code = fs_mkdir(pathbuf);
  snprintf(pathbuf, BUFSIZ, "%s/%s", path, MGIT_OBJECTS_FOLDER);
  return_code = fs_mkdir(pathbuf);

  return return_code;
}

int hash_object(const char* path) {
  mgit_hash_t hash = {0};
  char hash_hex[HASH_STRING_BYTES] = {0};
  size_t n_read = 0;
  char buff[BUFSIZ] = {0};
  char f_dest_name[BUFSIZ];

  get_file_hash(hash, path);

  for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
    sprintf(&hash_hex[i * 2], "%02x", hash[i]);
  }

  snprintf(f_dest_name, BUFSIZ, "%s%s", MGIT_OBJECTS_FOLDER, hash_hex);

  FILE* dest_f = fopen(f_dest_name, "w");
  if (!dest_f) {
    return MGIT_FILE_WRITE_ERROR;
  }
  FILE* src_f = fopen(path, "r");
  if (!src_f) {
    return MGIT_FILE_OPEN_ERROR;
  }

  while ((n_read = fread(buff, sizeof(char), BUFSIZ, src_f)) != 0) {
    if (fwrite(buff, sizeof(char), n_read, dest_f) != n_read) {
      fclose(src_f);
      fclose(dest_f);
      return MGIT_FILE_WRITE_ERROR;
    };
  }
  fclose(src_f);
  fclose(dest_f);
  return MGIT_SUCCESS;
}

int get_file_hash(mgit_hash_t dest_hash, const char* path) {
  char buff[BUFSIZ] = {'\0'};
  size_t read_len;
  SHA256_CTX sha256_ctx;

  FILE* f = fopen(path, "r");
  if (!f) {
    return MGIT_FILE_OPEN_ERROR;
  }
  SHA256_Init(&sha256_ctx);
  while ((read_len = fread(buff, sizeof(char), BUFSIZ, f)) > 0) {
    SHA256_Update(&sha256_ctx, buff, read_len);
  }

  fclose(f);
  SHA256_Final(dest_hash, &sha256_ctx);

  return MGIT_SUCCESS;
}

int cat_object(const char* hash, file_handler* handler) {
  char buff[BUFSIZ];
  snprintf(buff, BUFSIZ, "%s%s", MGIT_OBJECTS_FOLDER, hash);
  FILE* f = fopen(buff, "r");

  if (!f) {
    return MGIT_FILE_OPEN_ERROR;
  }
  handler(f);

  fclose(f);

  return MGIT_SUCCESS;
}
