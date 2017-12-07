#include <unistd.h>
#include <sys/syscall_codes.h>

ssize_t write(int fd, const void *buf, size_t count) {
    return syscall4(SYSCALL_WRITE, (uint64_t) fd, (uint64_t) buf, (uint64_t) count);
}

