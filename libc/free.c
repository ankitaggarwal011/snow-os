#include <stdlib.h>
#include <unistd.h>

void free(void *ptr) {
    syscall2(SYSCALL_FREE, (uint64_t) ptr);
}