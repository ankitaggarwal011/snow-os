#include <unistd.h>

int open(const char *pathname, int flags) {
    return (int) syscall3(SYSCALL_OPEN, (uint64_t) pathname, (uint64_t) flags);
}