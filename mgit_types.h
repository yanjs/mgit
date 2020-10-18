#ifndef MGIT_TYPES_H
#define MGIT_TYPES_H

#include <stdio.h>

#define MGIT_SUCCESS 0
#define MGIT_ARGUMENT_ERROR 1
#define MGIT_FILE_OPEN_ERROR 2
#define MGIT_FILE_WRITE_ERROR 3

#define MGIT_HASH_LENGTH 32

typedef int cmd_handler(int argc, const char* argv[]);
typedef int file_handler(FILE* fp);
typedef unsigned char mgit_hash_t[MGIT_HASH_LENGTH];
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

struct mgit_cmd {
  const char* cmd_name;
  cmd_handler* handler;
};

#endif  // MGIT_TYPES_H
