#include <unistd.h>

char *getcwd(char *buf, size_t size) {
    syscall3(SYSCALL_GETCWD, (uint64_t) buf, size);
    return buf;
}