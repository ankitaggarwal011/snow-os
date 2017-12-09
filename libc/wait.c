#include <unistd.h>

pid_t wait(int *status) {
    return syscall1(SYSCALL_WAIT);
}
