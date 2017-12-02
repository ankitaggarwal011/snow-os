#include <sys/defs.h>
#include <sys/kprintf.h>
#include <sys/kthread.h>
#include <sys/syscall_codes.h>
#include <sys/process.h>

uint64_t handle_syscall(syscall_code_t code, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5, uint64_t arg6) {
    switch (code) {
        case SYSCALL_WRITE: {
            kthread_t *cur_kt = get_cur_kthread();
            file_object_t fo = cur_kt->fds[arg2];
            file_sys_impl_t *fs_impl = fo.file_sys_impl;
            if (fs_impl == NULL) {
                kprintf("#%d: this file descriptor isn't handled for write\n", arg2);
                return;
            }
            fs_impl->write_impl((void *) arg3, arg4);
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