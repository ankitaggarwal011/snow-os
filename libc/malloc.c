#include <stdlib.h>

void *malloc(size_t size) {
    return syscall2(SYSCALL_MALLOC, size);
}