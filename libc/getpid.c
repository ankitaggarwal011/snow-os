#include <unistd.h>

pid_t getpid() {
    return syscall1(SYSCALL_PID);
}