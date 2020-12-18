#ifndef MGIT_TYPES_H_
#define MGIT_TYPES_H_

#define NORETURN
#define IN
#define OUT

#define SUCCESS 0
#define OPEN_FAILURE 1
#define READ_FAILURE 2
#define WRITE_FAILURE 3
#define MALLOC_FAILURE 4

#define CMD_NORMAL 0x0
#define CMD_LOAD_DB 0x1

#define FILE_READ_BYTES 8192

#define HASH_SIZE 32
#define HASH_STR_SIZE 65
#define MAX_FILENAME_SIZE 1024

typedef int cmd_t(int argc, const char* argv[]);

struct mgit_cmd {
  const char* name;
  cmd_t* handler;
  int type;
};

#endif