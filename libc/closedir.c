#include <dirent.h>
#include <unistd.h>

int closedir(DIR *dirp) {
    return syscall2(SYSCALL_CLOSEDIR, dirp->stream);
}
