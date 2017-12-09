#include <unistd.h>

pid_t wait(int *status) {
    return syscall2(SYSCALL_WAIT);
}
