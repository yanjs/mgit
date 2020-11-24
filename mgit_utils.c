#include "mgit_utils.h"

int mgit_die() {
  perror("mgit");
  exit(-1);
}

int mgit_die_errno(int errno) { exit(errno); }