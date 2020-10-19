#include "objects.h"

#include <dirent.h>
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
  };
  const char* p;
  for (p = path; *p != '\0'; ++p)
    ;
  for (; *p != '/'; --p)
    ;
  ++p;
  for (size_t i = 0; i < (sizeof(should_be_ignored) / sizeof(const char*));
       ++i) {
    if ((strcmp(p, should_be_ignored[i])) == 0) {
      return 1;
    }
  }
  return 0;
}

int hash_to_hex(mgit_hash_string_t* dest, mgit_hash_t* hash) {
  for (size_t i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
    sprintf(&dest[i * 2], "%02x", hash[i]);
  }
}

static int fs_listdir_rec(mgit_hash_t* hash, const char* path, size_t depth) {
  // TODO: redesign this method
  if (is_ignored(path)) {
    return 0;
  }
  DIR* dir = opendir(path);
  if (dir == NULL) {
    return 0;
  }
  struct dirent* curr;
  struct stat curr_stat;
  struct mgit_dirent child;
  SHA256_CTX sha256_ctx;
  mgit_buffer_t buffer, buffer_dest;
  mgit_hash_string_t hash_hex;
  snprintf(buffer, MGIT_BUFSIZ, "%s%zd", MGIT_DIRS_FOLDER, depth);

  SHA256_Init(&sha256_ctx);

  FILE* f = fopen(buffer, "w");
  if (!f) return MGIT_FILE_OPEN_ERROR;

  while ((curr = readdir(dir)) != NULL) {
    lstat(curr->d_name, &curr_stat);
    puts(curr->d_name);
    if (S_ISDIR(curr_stat.st_mode)) {
      // TODO
      child.type = MGIT_DIRECTORY;
      fs_listdir_rec(&child.hash, curr->d_name, depth + 1);
    } else if (S_ISREG(curr_stat.st_mode)) {
      if (!is_ignored(curr->d_name)) {
        child.type = MGIT_FILE;
        get_file_hash(&child.hash[0], curr->d_name);
      }
    } else {
      printf("Mgit does not support this file: %s", curr->d_name);
      continue;
    }
    SHA256_Update(&sha256_ctx, &child, sizeof(child));
    fwrite(&child, sizeof(child), 1, f);
  }
  SHA256_Final((unsigned char*)hash, &sha256_ctx);

  hash_to_hex(&hash_hex, &hash);
  puts(hash_hex);
  strncpy(buffer_dest, path, MGIT_BUFSIZ);
  strncat(buffer_dest, hash_hex, MGIT_BUFSIZ);

  rename(buffer, buffer_dest);
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

int hash_object(const char* path) {
  mgit_hash_t hash = {0};
  char hash_hex[MGIT_HASH_STRING_BYTES] = {0};
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
  SHA256_Final(hash, &sha256_ctx);

  for (size_t i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
    snprintf(&hash_hex[i * 2], MGIT_HASH_STRING_BYTES, "%02x", hash[i]);
  }
  puts(hash_hex);
  strncat(f_dest_name, hash_hex, MGIT_BUFSIZ);

  fclose(src_f);
  fclose(dest_f);
  rename(MGIT_OBJECTS_FOLDER "temp", f_dest_name);

  return MGIT_SUCCESS;
}

int get_file_hash(mgit_hash_t hash_dest, const char* path) {
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
  SHA256_Final(hash_dest, &sha256_ctx);

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
