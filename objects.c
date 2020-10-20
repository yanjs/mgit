#include "objects.h"

#include <dirent.h>
#include <libgen.h>
#include <openssl/sha.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

static const char* mgit_directories[] = {
    MGIT_FOLDER,
    MGIT_OBJECTS_FOLDER,
    MGIT_DIRS_FOLDER,
};

/**
 * Make a directory at this path.
 */
int fs_mkdir(const char* path) {
  return mkdir(path, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
}

/**
 * Return if the file at this path should be ignored.
 */
int is_ignored(const char* path) {
  const char* should_be_ignored[] = {
      ".mgit",
      ".",
      "..",
  };
  char copy_of_path[MGIT_BUFSIZ];
  strncpy(copy_of_path, path, MGIT_BUFSIZ);
  char* bn = basename(copy_of_path);

  for (size_t i = 0; i < sizeof(should_be_ignored) / sizeof(const char*); ++i) {
    if ((strcmp(bn, should_be_ignored[i])) == 0) {
#ifdef MGIT_DEBUG
      fputs("Ignore: ", stdout);
      puts(path);
#endif  // MGIT_DEBUG
      return 1;
    }
  }
  return 0;
}

int hash_to_hex(mgit_hash_string_t* dest, mgit_hash_t* hash) {
  for (size_t i = 0; i < MGIT_HASH_LENGTH; ++i) {
    sprintf(&dest->value[i * 2], "%02x", hash->value[i]);
  }
  return 0;
}

static int fs_listdir_rec(mgit_hash_t* hash, const char* path, size_t depth) {
  // TODO: redesign this method
  DIR* dir = opendir(path);
  if (dir == NULL) {
    return 0;
  }
  struct dirent* curr;
  struct stat curr_stat;
  struct mgit_dirent child;
  SHA256_CTX sha256_ctx;
  mgit_buffer_t src_str, dest_str, curr_str;
  char* pbuffer_curr_next;
  mgit_hash_string_t hash_hex;
  snprintf(src_str.value, MGIT_BUFSIZ, "%s%zd", MGIT_DIRS_FOLDER, depth);

  SHA256_Init(&sha256_ctx);

  FILE* f = fopen(src_str.value, "w");
  if (!f) return MGIT_FILE_OPEN_ERROR;

  strncpy(curr_str.value, path, MGIT_BUFSIZ);
  pbuffer_curr_next = curr_str.value + strlen(curr_str.value);
  if (pbuffer_curr_next != curr_str.value) (*pbuffer_curr_next) = '/';
  ++pbuffer_curr_next;

  while ((curr = readdir(dir)) != NULL) {
    strncpy(pbuffer_curr_next, curr->d_name, MGIT_BUFSIZ);
    lstat(curr_str.value, &curr_stat);
    //#ifdef MGIT_DEBUG
    printf("Current: %s\n", curr_str.value);
    fflush(stdout);
    //#endif  // MGIT_DEBUG
    if (is_ignored(curr_str.value)) {
      continue;
    }
    if (S_ISDIR(curr_stat.st_mode)) {
      child.type = MGIT_DIRECTORY;
      fs_listdir_rec(&child.hash, curr_str.value, depth + 1);
    } else if (S_ISREG(curr_stat.st_mode)) {
      child.type = MGIT_FILE;
      hash_object(&child.hash, curr_str.value);
    } else {
      printf("Mgit does not support this file: %s\n", curr_str.value);
      continue;
    }
    SHA256_Update(&sha256_ctx, &child, sizeof(child));
    fwrite(&child, sizeof(child), 1, f);
  }
  closedir(dir);
  SHA256_Final(hash->value, &sha256_ctx);

  hash_to_hex(&hash_hex, hash);
  strncpy(dest_str.value, MGIT_DIRS_FOLDER, MGIT_BUFSIZ);
  strncat(dest_str.value, hash_hex.value, MGIT_BUFSIZ);

  rename(src_str.value, dest_str.value);
  return 0;
}

int fs_listdir(const char* path) {
  //
  mgit_hash_t hash;
  return fs_listdir_rec(&hash, path, 0);
}

/**
 * Initialize a .mgit directory here.
 */
int init_mgit_dir() {
  int return_code;

  for (size_t i = 0; i < sizeof(mgit_directories) / sizeof(const char*); ++i) {
    return_code = fs_mkdir(mgit_directories[i]);
  }
  return return_code;
}

int hash_object(mgit_hash_t* hash, const char* path) {
  mgit_hash_string_t hash_hex;
  size_t n_read = 0;
  char buff[MGIT_BUFSIZ] = {0};
  char f_dest_name[MGIT_BUFSIZ] = MGIT_OBJECTS_FOLDER;
  SHA256_CTX sha256_ctx;

  FILE* dest_f = fopen(MGIT_OBJECTS_FOLDER "temp", "w");

  if (!dest_f) {
    return MGIT_FILE_WRITE_ERROR;
  }
  FILE* src_f = fopen(path, "r");
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
  rename(MGIT_OBJECTS_FOLDER "temp", f_dest_name);

  return MGIT_SUCCESS;
}

int get_file_hash(mgit_hash_t* hash_dest, const char* path) {
  char buff[MGIT_BUFSIZ] = {'\0'};
  size_t read_len;
  SHA256_CTX sha256_ctx;

  FILE* f = fopen(path, "r");
  if (!f) {
    return MGIT_FILE_OPEN_ERROR;
  }
  SHA256_Init(&sha256_ctx);
  while ((read_len = fread(buff, sizeof(char), MGIT_BUFSIZ, f)) > 0) {
    SHA256_Update(&sha256_ctx, buff, read_len);
  }

  fclose(f);
  SHA256_Final(&hash_dest->value[0], &sha256_ctx);

  return MGIT_SUCCESS;
}

int cat_object(const char* hash, file_handler* handler) {
  char buff[MGIT_BUFSIZ];
  snprintf(buff, MGIT_BUFSIZ, "%s%s", MGIT_OBJECTS_FOLDER, hash);
  FILE* f = fopen(buff, "r");

  if (!f) {
    return MGIT_FILE_OPEN_ERROR;
  }
  handler(f);

  fclose(f);

  return MGIT_SUCCESS;
}
