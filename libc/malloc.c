#include <stdlib.h>
#include <unistd.h>

void *malloc(size_t size) {
    return (void *) syscall2(SYSCALL_MALLOC, size);
}