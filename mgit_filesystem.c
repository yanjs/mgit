#include "mgit_filesystem.h"

#include "mgit_types.h"

#ifndef MGIT_TESTING

#ifdef __linux__
#include <sys/stat.h>
#include <sys/types.h>

int mgit_mkdir() {
  const char dirname = "./.mgit";
  int ret = mkdir(dirname, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
  if (ret) {
    perror(dirname);
    return MGIT_FILE_WRITE_ERROR;
  }
  return MGIT_SUCCESS;
}
#endif

size_t mgit_file_read(void* buffer, size_t size, FILE* f) {
  size_t elem_read = fread(buffer, size, 1, f);
  return elem_read;
}

int mgit_file_write(FILE* f, size_t size, const unsigned char* bytes) {
  size_t elem_written = fwrite(bytes, sizeof(unsigned char), size, bytes);
  if (elem_written != size) {
    perror("Write to file failed");
    return MGIT_FILE_WRITE_ERROR;
  }
  return MGIT_SUCCESS;
}

int mgit_file_move(const char* dest, const char* src) {
  int ret = rename(src, dest);
  if (ret) {
    perror("Move file failed");
    return MGIT_FILE_WRITE_ERROR;
  }
  return MGIT_SUCCESS;
}

int mgit_file_copy(const char* dest, const char* src) {
  FILE* srcf = mgit_file_open_r(src);
  if (!srcf) return MGIT_FILE_OPEN_ERROR;
  FILE* destf = mgit_file_open_w(dest);
  if (!destf) {
    fclose(srcf);
    return MGIT_FILE_OPEN_ERROR;
  }
  size_t nread;
  int write;
  mgit_buffer_t buffer;
  do {
    nread = mgit_file_read(buffer.value, MGIT_BUFSIZ, srcf);
    if (mgit_file_write(destf, buffer.value, nread)) {
      goto return_write_error;
    };
    if (nread == 0 && !feof(srcf)) {
      goto return_write_error;
    }
  } while (nread > 0);
  fclose(srcf);
  fclose(destf);
  return MGIT_SUCCESS;
return_write_error:
  fclose(srcf);
  fclose(destf);
  return MGIT_FILE_WRITE_ERROR;
}

FILE* mgit_file_open_r(const char* file) { return fopen(file, "r"); }
FILE* mgit_file_open_w(const char* file) { return fopen(file, "w"); }
int mgit_file_close(FILE* f) {
  int ret = fclose(f);
  if (ret) {
    return MGIT_FILE_WRITE_ERROR;
  }
  return MGIT_SUCCESS;
}

#endif