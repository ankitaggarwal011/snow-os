#include <dirent.h>
#include <unistd.h>
#include <stdlib.h>

struct dirent read_dirent;

struct dirent *readdir(DIR *dirp) {
    read_dirent.d_name = "";
    uint64_t stream_addr = dirp->stream;
    if (syscall3(SYSCALL_READDIR, stream_addr, (uint64_t) &read_dirent.d_name) == -1) {
        return NULL;
    }
    return (struct dirent *)((uint64_t) &read_dirent);
}
