#include <unistd.h>

void yield() {
    return syscall1(SYSCALL_YIELD);
}