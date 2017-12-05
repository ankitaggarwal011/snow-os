#include <stdlib.h>

void free(void *ptr) {
    return syscall2(SYSCALL_FREE, ptr);
}