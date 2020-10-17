
#include "filesystem.h"

#include <dirent.h>

#include "objects.h"

#ifdef __linux__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

int fs_mkdir(const char *path) {
  return mkdir(path, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
}

static int is_ignored(const char *path) {
  const char *ignored[] = {
      ".mgit",
  };
  char buff[BUFSIZ];
  char *p;
  for (p = path; *p != '\0'; ++p)
    ;
  for (; *p != '/'; --p)
    ;
  ++p;
  strncpy(buff, p, BUFSIZ);
  for (size_t i = 0; i < (sizeof(ignored) / sizeof(const char *)); ++i) {
    if ((strcmp(buff, ignored[i])) == 0) {
      return 1;
    }
  }
  return 0;
}

int fs_listdir(const char *path) {
  // TODO: redesign this method
  if (is_ignored(path)) {
    return 0;
  }
  DIR *dir = opendir(path);
  if (dir == NULL) {
    return 0;
  }
  struct dirent *curr;
  struct stat curr_stat;
  size_t max_nchildren = 16;
  struct mgit_dirent *children =
      calloc(max_nchildren, sizeof(struct mgit_dirent));
  size_t nchildren = 0;

  while ((curr = readdir(dir)) != NULL) {
    lstat(curr->d_name, &curr_stat);
    if (S_ISDIR(curr_stat.st_mode)) {
      // TODO
      fs_listdir(curr->d_name);
    } else if (S_ISREG(curr_stat.st_mode)) {
      if (!is_ignored(curr->d_name)) {
        children[nchildren].type = MGIT_FILE;
        get_file_hash(&children[nchildren].hash, curr->d_name);
      }
    } else {
      printf("Mgit does not support this file: %s", curr->d_name);
    }
  }
}

#endif  // __linux__
