#ifndef MGIT_FILESYSTEM_H
#define MGIT_FILESYSTEM_H

#include <stdio.h>

int mgit_mkdir();
int mgit_file_write(const unsigned char* bytes);
int mgit_file_move(const char* dest, const char* src);
int mgit_file_copy(const char* dest, const char* src);
FILE* mgit_file_open_r(const char* file);
FILE* mgit_file_open_w(const char* file);
int mgit_file_close(FILE* f);

#endif