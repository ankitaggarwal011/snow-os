#include <unistd.h>

int waitpid(int pid, int *status) {
    return syscall2(SYSCALL_WAITPID, pid);
}
