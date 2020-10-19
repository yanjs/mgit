#ifndef MGIT_EXPERIMENTS_H
#define MGIT_EXPERIMENTS_H

#include <openssl/sha.h>

#include "mgit_types.h"
#include "objects.h"

typedef unsigned char mgit_file_block[BUFSIZ];

typedef int mgit_file_block_handler(size_t sizeof_block,
                                    const mgit_file_block block);
typedef int mgit_file_block_before();
typedef int mgit_file_block_after();
struct mgit_file_block_handlers {
  mgit_file_block_before* before;
  mgit_file_block_handler* handler;
  mgit_file_block_after* after;
};

int for_each_in_file(const char* path, size_t nfbs, ...);
extern struct mgit_file_block_handlers g_mgit_fbs_hash;
const char* mgit_fbs_get_file_hash_hex();
const unsigned char* mgit_fbs_get_file_hash();
extern struct mgit_file_block_handlers g_mgit_fbs_copy;

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

#endif