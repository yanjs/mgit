
#include "fs.h"

#include "utils.h"

#ifdef __linux__
#include <sys/stat.h>
#endif

/**
 * Make a directory at this path. Die if failed.
 */
void fs_mkdir(const char* path) {
#ifdef __linux__
  if (mkdir(path, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH) != 0) {
    die();
  }
#endif
}