#include <unistd.h>

pid_t getppid() {
    return syscall1(SYSCALL_PPID);
}