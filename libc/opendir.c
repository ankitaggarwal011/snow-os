#include <dirent.h>
#include <unistd.h>
#include <stdlib.h>

DIR dir_open;

DIR *opendir(const char *name) {
    dir_open.stream = syscall2(SYSCALL_OPENDIR, (uint64_t) name);
    return (DIR *)((uint64_t) &dir_open);
}
