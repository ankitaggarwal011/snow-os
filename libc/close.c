#include <unistd.h>

int close(int fd) {
    return (int) syscall2(SYSCALL_CLOSE, (uint64_t) fd);
}