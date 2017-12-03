#include <unistd.h>

extern uint64_t syscall1(syscall_code_t code);

int fork() {
    return syscall1(SYSCALL_FORK);
}