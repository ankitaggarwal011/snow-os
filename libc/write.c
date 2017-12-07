#include <unistd.h>

ssize_t write(int fd, const void *buf, size_t count) {
    uint32_t ret_val = syscall4(SYSCALL_WRITE, (uint64_t) fd, (uint64_t) buf, (uint64_t) count);
    return ret_val;
}
