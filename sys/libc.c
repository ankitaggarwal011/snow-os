#include <sys/defs.h>
//#include <unistd.h>
#include <sys/syscall_codes.h>

/*
int open(const char *pathname, int flags);
int close(int fd);
ssize_t read(int fd, void *buf, size_t count);

int unlink(const char *pathname);
*/


extern uint64_t syscall0();

extern uint64_t syscall1(syscall_code_t code);

extern uint64_t syscall2(syscall_code_t code, uint64_t arg2);

extern uint64_t syscall3(syscall_code_t code, uint64_t arg2, uint64_t arg3);

extern uint64_t syscall4(syscall_code_t code, uint64_t arg2, uint64_t arg3, uint64_t arg4);

extern uint64_t syscall5(syscall_code_t code, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5);

extern uint64_t syscall6(syscall_code_t code, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg6);

ssize_t write(int fd, const void *buf, size_t count) {
    return (ssize_t) syscall4(SYSCALL_WRITE, (uint64_t) fd, (uint64_t) buf, (uint64_t) count);
}

ssize_t read(int fd, const void *buf, size_t count) {
    return (ssize_t) syscall4(SYSCALL_READ, (uint64_t) fd, (uint64_t) buf, (uint64_t) count);
}

int open(const char *pathname, int flags) {
    return (int) syscall3(SYSCALL_OPEN, (uint64_t) pathname, (uint64_t) flags);
}

