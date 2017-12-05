#include <stdlib.h>
#include <unistd.h>

void free(void *ptr) {
    return syscall2(SYSCALL_FREE, ptr);
}