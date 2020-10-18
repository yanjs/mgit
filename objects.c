#include "objects.h"

#include <openssl/sha.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "filesystem.h"

static const char* mgit_directories[] = {
    MGIT_FOLDER,
    MGIT_OBJECTS_FOLDER,
    MGIT_DIRS_FOLDER,
};

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

// mgit hash suite start
static SHA256_CTX sha256_ctx;
static mgit_hash_t sha256_result;
static char sha256_hex_result[MGIT_HASH_STRING_BYTES] = "";
static int mgit_fbs_sha256_before() {
  SHA256_Init(&sha256_ctx);
  return MGIT_SUCCESS;
}
static int mgit_fbs_sha256_handler(size_t sizeof_block,
                                   const mgit_file_block fb) {
  SHA256_Update(&sha256_ctx, fb, sizeof_block);
  return MGIT_SUCCESS;
}
static int mgit_fbs_sha256_after() {
  SHA256_Final(sha256_result, &sha256_ctx);
  return MGIT_SUCCESS;
}
struct mgit_file_block_handlers g_mgit_fbs_hash = {
    mgit_fbs_sha256_before,
    mgit_fbs_sha256_handler,
    mgit_fbs_sha256_after,
};
const char* mgit_fbs_get_file_hash_hex() {
  if (sha256_hex_result[0] == '\0') {
    for (size_t i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
      sprintf(&sha256_hex_result[i * 2], "%02x", sha256_result[i]);
    }
  }
  return sha256_hex_result;
}
const unsigned char* mgit_fbs_get_file_hash() { return sha256_result; }
// mgit hash suite end

// mgit copy suite start
static char* copy_dest_path = MGIT_OBJECTS_FOLDER "temp";
static FILE* copy_dest_f;
static int mgit_fbs_copy_before() {
  copy_dest_f = fopen(copy_dest_path, "w");
  if (copy_dest_f == NULL) {
    perror("mgit_fbs_copy_before");
    return MGIT_FILE_OPEN_ERROR;
  }
  return MGIT_SUCCESS;
}
static int mgit_fbs_copy_handler(size_t sizeof_block,
                                 const mgit_file_block fb) {
  if (fwrite(fb, sizeof(unsigned char), sizeof_block, copy_dest_f) !=
      sizeof_block) {
    return MGIT_FILE_WRITE_ERROR;
  }
  return MGIT_SUCCESS;
}
static int mgit_fbs_copy_after() {
  fclose(copy_dest_f);
  return MGIT_SUCCESS;
}
struct mgit_file_block_handlers g_mgit_fbs_copy = {
    mgit_fbs_copy_before,
    mgit_fbs_copy_handler,
    mgit_fbs_copy_after,
};
// mgit copy suite end

int for_each_in_file(const char* path, size_t nfbs, ...) {
  FILE* f = fopen(path, "r");
  if (f == NULL) {
    return MGIT_FILE_OPEN_ERROR;
  }
  va_list vl;
  int result = 0;
  size_t read_len;
  unsigned char buff[BUFSIZ];
  struct mgit_file_block_handlers* curr_handlers;

  va_start(vl, nfbs);
  for (size_t i = 0; i < nfbs; ++i) {
    curr_handlers = va_arg(vl, struct mgit_file_block_handlers*);
    if (curr_handlers != NULL && curr_handlers->before != NULL) {
      if (curr_handlers->before() != MGIT_SUCCESS) {
        curr_handlers->handler = NULL;
        curr_handlers->after = NULL;
        result = -1;
      };
    }
  }

  while ((read_len = fread(buff, sizeof(char), BUFSIZ, f)) > 0) {
    va_start(vl, nfbs);
    for (size_t i = 0; i < nfbs; ++i) {
      curr_handlers = va_arg(vl, struct mgit_file_block_handlers*);
      if (curr_handlers != NULL && curr_handlers->handler != NULL) {
        if (curr_handlers->handler(read_len, buff) != MGIT_SUCCESS) {
          curr_handlers->handler = NULL;
          result = -1;
        }
      }
    }
  }

  va_start(vl, nfbs);
  for (size_t i = 0; i < nfbs; ++i) {
    curr_handlers = va_arg(vl, struct mgit_file_block_handlers*);
    if (curr_handlers != NULL && curr_handlers->after != NULL) {
      curr_handlers->after();
    }
  }

  va_end(vl);
  fclose(f);
  return result;
}

int hash_object(const char* path) {
  mgit_hash_t hash = {0};
  char hash_hex[MGIT_HASH_STRING_BYTES] = {0};
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

int get_file_hash(mgit_hash_t hash_dest, const char* path) {
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
  SHA256_Final(hash_dest, &sha256_ctx);

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
