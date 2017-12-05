#include <dirent.h>
#include <unistd.h>

DIR *opendir(const char *name) {
    DIR *dir_open = malloc(sizeof(DIR));
    dir_open->stream = syscall2(SYSCALL_OPENDIR, (uint64_t) name);
    return dir_open;
}
