#include <unistd.h>
#include <sys/syscall_codes.h>

ssize_t read(int fd, void *buf, size_t count) {
    if (fd == 0) {
        yield();
    }
    uint64_t ret_val = syscall4(SYSCALL_READ, (uint64_t) fd, (uint64_t) buf, (uint64_t) count);
    return (ssize_t) ret_val;
}
