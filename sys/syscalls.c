#include <sys/defs.h>
#include <sys/kprintf.h>
#include <sys/kthread.h>
#include <sys/syscall_codes.h>
#include <sys/process.h>

uint64_t handle_syscall(syscall_code_t code, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5, uint64_t arg6) {
    switch (code) {
        case SYSCALL_WRITE:
            if (arg2 == 1) {
                char *str = (char *) arg3;
                for (int i = 0; i < arg4; i++) {
                    kprintf("%c", str[i]);
                }
            } else {
                kprintf("Wrong buff for write syscall\n");
            }
            return arg4;
        case SYSCALL_FORK:
            return fork();
        default:
            kprintf("Arg1: %x, Arg2: %x, Arg3: %x \n", code, arg2, arg3);
            kprintf("Arg4: %x, Arg5: %x, Arg6: %x \n", arg4, arg5, arg6);
    }
    return 0;
}