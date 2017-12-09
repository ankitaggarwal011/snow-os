#include <sys/defs.h>
#include <sys/syscall_codes.h>
#include <unistd.h>

void get_process_state(char *buf) {
    syscall2(SYSCALL_PS, (uint64_t) buf);
}