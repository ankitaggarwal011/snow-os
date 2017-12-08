#include <unistd.h>
#include <sys/syscall_codes.h>

int kill(int pid) {
    return syscall2(SYSCALL_KILL, (uint64_t) pid);
}