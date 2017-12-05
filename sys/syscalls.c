#include <sys/defs.h>
#include <sys/kprintf.h>
#include <sys/kthread.h>
#include <sys/process.h>
#include <sys/syscall_codes.h>
#include <sys/process.h>
#include <sys/vfs.h>
#include <sys/tarfs.h>

uint64_t handle_syscall(syscall_code_t code, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5, uint64_t arg6) {
    switch (code) {
        case SYSCALL_WRITE: {
            kthread_t *cur_kt = get_current_process();
            file_object_t *fo = cur_kt->fds[arg2];
            file_sys_impl_t *fs_impl = fo->file_sys_impl;
            if (fs_impl == NULL) {
                kprintf("#%d: this file descriptor isn't handled for write\n", arg2);
                return 0;
            }
            fs_impl->write_impl((void *) arg3, arg4);
        }
            return arg4;
        case SYSCALL_FORK:
            return fork();

        case SYSCALL_READ: {
            //hack
            char *file = (char *) get_file("lib/crt1.o");
            return tarfs_read((void *) arg3, arg4, file, 0);
            /*kthread_t *cur_kt = get_cur_kthread();
            file_object_t *fo = cur_kt->fds[arg2];
            file_sys_impl_t *fs_impl = fo->file_sys_impl;
            if (fs_impl == NULL) {
                kprintf("#%d: this file descriptor isn't handled for read\n", arg2);
                return 0;
            }
            fs_impl->read_impl((void *) arg3, arg4); */
        }

        case SYSCALL_OPEN: {
            char *file = (char *) get_file((char *) arg2);
            if (file == NULL) {
                return -1;
            }
            kthread_t *cur_kt = get_current_process();
            int fd_idx = 3; // start searching from 3
            for (; fd_idx < NUM_FDS; fd_idx++) {
                if (cur_kt->fds[fd_idx] == NULL) {
                    break;
                }
            }
            if (fd_idx == NUM_FDS) {
                // fd limit reached
                return -2;
            }
            // now, make new file object
            /*file_object_t *file_object_t = (file_object_t *) kmalloc(sizeof(file_object_t));
            file_object_t->name = (char *) arg2;
            file_object_t->offset = 0;
            file_object_t->fs_impl =*/
            // do later
        }
            break;
        case SYSCALL_YIELD:
            scheduler();
            break;
        case SYSCALL_PID:
            return get_process_pid();
        case SYSCALL_PPID:
            return get_process_ppid();
        case SYSCALL_MALLOC:
            return user_malloc(arg2);
        case SYSCALL_FREE:
            user_free(arg2);
            break;
        case SYSCALL_OPENDIR:
            return open_dir((char *) arg2);
        case SYSCALL_READDIR:
            return read_dir(arg2, (char *) arg3);
        case SYSCALL_CLOSEDIR:
            return close_dir(arg2);
        default:
            kprintf("Arg1: %x, Arg2: %x, Arg3: %x \n", code, arg2, arg3);
            kprintf("Arg4: %x, Arg5: %x, Arg6: %x \n", arg4, arg5, arg6);
    }
    return 0;
}