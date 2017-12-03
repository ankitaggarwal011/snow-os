#include <unistd.h>

int yield() {
    return syscall1(SYSCALL_YIELD);
}