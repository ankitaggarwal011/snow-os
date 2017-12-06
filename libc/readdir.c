#include <dirent.h>
#include <unistd.h>
#include <stdlib.h>

struct dirent read_dirent;

struct dirent *readdir(DIR *dirp) {
    for (int i = 0; i < 256; i++) read_dirent.d_name[i] = 0;
    DIR dirp_copy;
    dirp_copy = dirp->stream;
    if (syscall3(SYSCALL_READDIR, dirp_copy.stream, (uint64_t) &read_dirent.d_name) == -1) {
        return NULL;
    }
    return (struct dirent *)((uint64_t) &read_dirent);
}
