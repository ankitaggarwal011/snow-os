#include <dirent.h>
#include <unistd.h>

DIR *opendir(const char *name) {
    DIR dir_open;
    dir_open.stream = syscall2(SYSCALL_OPENDIR, name);
    return (DIR *) dir_open;
}
