
#include "mgit_objects.h"

#include <openssl/sha.h>
#include <stdio.h>

int mgit_file_is_ignored(mgit_path_t *path) {
  const char *should_be_ignored[] = {
      ".mgit",
      ".git",
      ".",
      "..",
  };

  for (size_t i = 0; i < sizeof(should_be_ignored) / sizeof(const char *);
       ++i) {
    if ((strcmp(mgit_path_peek(path), should_be_ignored[i])) == 0) {
#ifdef MGIT_DEBUG
      fputs("Ignore: ", stdout);
      puts(path);
#endif  // MGIT_DEBUG
      return 1;
    }
  }
  return 0;
}

int mgit_hex_from_hash(mgit_hash_string_t *hash_hex, mgit_hash_t *hash) {
  for (size_t i = 0; i < MGIT_HASH_LENGTH; ++i) {
    sprintf(&hash_hex->value[i * 2], "%02x", hash->value[i]);
  }
  return 0;
}

int mgit_object_store(mgit_hash_t *hash, mgit_path_t *path) {
  mgit_hash_string_t hash_hex;
  size_t n_read = 0;
  SHA256_CTX sha256_ctx;
  unsigned char buff[MGIT_BUFSIZ];

  FILE *dest_f = fopen(MGIT_DIR_OBJECTS "temp", "w");

  if (!dest_f) {
    return MGIT_FILE_WRITE_ERROR;
  }
  FILE *src_f = fopen(path->value, "r");
  if (!src_f) {
    fclose(dest_f);
    return MGIT_FILE_OPEN_ERROR;
  }

  SHA256_Init(&sha256_ctx);

  while ((n_read = fread(buff, sizeof(char), MGIT_BUFSIZ, src_f)) != 0) {
    if (fwrite(buff, sizeof(char), n_read, dest_f) != n_read) {
      fclose(src_f);
      fclose(dest_f);
      return MGIT_FILE_WRITE_ERROR;
    };
    SHA256_Update(&sha256_ctx, buff, n_read);
  }
  SHA256_Final(&hash->value[0], &sha256_ctx);
  hash_to_hex(&hash_hex, hash);
  strncat(f_dest_name, hash_hex.value, MGIT_BUFSIZ);

  fclose(src_f);
  fclose(dest_f);
  rename(MGIT_DIR_OBJECTS "temp", f_dest_name);

  return MGIT_SUCCESS;
}
int mgit_object_fetch(mgit_path_t *path, mgit_hash_t *hash);
int mgit_tree_store(mgit_hash_t *hash, mgit_path_t *path);
int mgit_tree_fetch(mgit_path_t *path, mgit_hash_t *hash);
