
#ifdef __linux__
#include <sys/types.h>
#include <sys/stat.h>

int fs_mkdir(const char* path) {
    return mkdir(path, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
}

#endif // __linux__
