#include <dirent.h>
#include <unistd.h>
#include <stdlib.h>

struct dirent *readdir(DIR *dirp) {
    struct dirent *read_dirent = (struct dirent *) malloc(sizeof(struct dirent));
    if (syscall3(SYSCALL_READDIR, dirp->stream, (uint64_t) &read_dirent->d_name) == -1) {
        return NULL;
    }
    return read_dirent;
}
