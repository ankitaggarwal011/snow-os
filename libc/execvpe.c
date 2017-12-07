#include <unistd.h>

int execvpe(const char *file, char *const argv[], char *const envp[]) {
    return syscall4(SYSCALL_EXECVPE, (uint64_t) file, (uint64_t) argv, (uint64_t) envp);
}
