#include <unistd.h>
#include <sys/syscall_codes.h>

void exit(int status) {
    syscall2(SYSCALL_EXIT, (uint64_t) status);
}