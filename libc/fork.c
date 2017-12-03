#include <unistd.h>

pid_t fork() {
    return syscall1(SYSCALL_FORK);
}