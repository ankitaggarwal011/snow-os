#include <unistd.h>

int chdir(const char *path) {
    return syscall2(SYSCALL_CHDIR, (uint64_t) path);
}