#include <dirent.h>
#include <unistd.h>
#include <stdlib.h>

DIR *opendir(const char *name) {
    DIR dir_open;
    dir_open.stream = syscall2(SYSCALL_OPENDIR, (uint64_t) name);
    return (DIR *)((uint64_t) dir_open);
}
