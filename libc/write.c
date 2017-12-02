#include <unistd.h>

extern uint64_t syscall4(syscall_code_t code, uint64_t arg2, uint64_t arg3, uint64_t arg4);

ssize_t write(int fd, const void *buf, size_t count) {
    return (ssize_t) syscall4(SYSCALL_WRITE, (uint64_t) fd, (uint64_t) buf, (uint64_t) count);
}
