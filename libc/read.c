#include <unistd.h>
ssize_t read(int fd, void *buf, size_t count) {
    return (ssize_t) syscall4(SYSCALL_READ, (uint64_t) fd, (uint64_t) buf, (uint64_t) count);
}
