#include "init.h"

#include "../db.h"
#include "../fs.h"

static int init() {
  fs_mkdir(g_mgit_dir);
  fs_mkdir(g_mgit_objects_dir);
  return SUCCESS;
}
int cmd_init(int argc, const char* argv[]) {
  init();
  return SUCCESS;
}