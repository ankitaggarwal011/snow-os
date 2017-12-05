#include <stdlib.h>
#include <unistd.h>

void *malloc(size_t size) {
    return syscall2(SYSCALL_MALLOC, size);
}