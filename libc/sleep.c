#include <unistd.h>

unsigned int sleep(unsigned int seconds) {
    return (unsigned int) syscall2(SYSCALL_SLEEP, (unsigned int) seconds);
}