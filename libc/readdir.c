#include <dirent.h>
#include <unistd.h>
#include <stdlib.h>

struct dirent read_dirent;

struct dirent *readdir(DIR *dirp) {
    if (syscall3(SYSCALL_READDIR, dirp->stream, (uint64_t) &read_dirent.d_name) == -1) {
        return NULL;
    }
    return (struct dirent *)((uint64_t) &read_dirent);
}
