#include <dirent.h>
#include <unistd.h>

struct dirent *readdir(DIR *dirp) {
    struct dirent read_dirent;
    if (syscall3(SYSCALL_READDIR, dirp->stream, (uint64_t) &read_dirent.d_name) == -1) {
        return NULL;
    }
    return (struct dirent *) ((uint64_t) &read_dirent);
}
