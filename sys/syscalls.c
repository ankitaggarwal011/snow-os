#include <sys/defs.h>
#include <sys/kprintf.h>
#include <sys/kthread.h>
#include <sys/process.h>
#include <sys/syscall_codes.h>
#include <sys/paging.h>
#include <sys/vfs.h>
#include <sys/tarfs.h>
#include <sys/sleep.h>

uint64_t
handle_syscall(syscall_code_t code, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5, uint64_t arg6) {
    switch (code) {
        case SYSCALL_WRITE: {
            kthread_t *cur_kt = get_current_process();
            if (arg2 < 0 || arg2 >= NUM_FDS) {
                kprintf("Invalid FD #, must be between %d and %d\n", 0, NUM_FDS);
                return -1;
            }
            if (arg2 == 0) {
                kprintf("Write op not supported on FD #%d", arg2);
                return -1;
            }
            file_object_t *fo = cur_kt->fds[arg2];
            if (fo == NULL) {
                kprintf("FD %d is not setup yet!\n", arg2);
                return -1;
            }
            file_sys_impl_t *fs_impl = fo->file_sys_impl;
            if (fs_impl == NULL) {
                kprintf("#%d: this file descriptor isn't handled for write\n", arg2);
                return -1;
            }
            if (fs_impl->write_impl == NULL) {
                kprintf("FD # %d does not support writing.\n", arg2);
                return -1;
            }
            return fs_impl->write_impl((void *) arg3, arg4, fo->content_start, fo->offset);
        }

        case SYSCALL_FORK: {
            fork();
            uint64_t fork_ret_val = -1;
            __asm__ __volatile__(
            "movq %%rax, %0;"
            :"=g"(fork_ret_val)
            );
            if (fork_ret_val == 0) {
                kprintf("Executing for child!!");
            }
            return fork_ret_val;
        }
        case SYSCALL_READ: {
            //hack
            kthread_t *cur_kt = get_current_process();
            if (arg2 < 0 || arg2 >= NUM_FDS) {
                kprintf("Invalid FD #, must be between %d and %d\n", 0, NUM_FDS);
                return -1;
            }
            if (arg2 == 1 || arg2 == 2) {
                kprintf("Read op not supported on FD #%d", arg2);
                return -1;
            }
            file_object_t *fo = cur_kt->fds[arg2];
            if (fo == NULL) {
                kprintf("FD $d is not setup yet!\n", arg2);
                return -1;
            }
            file_sys_impl_t *fs_impl = fo->file_sys_impl;
            if (fs_impl == NULL) {
                kprintf("#%d: this file descriptor isn't handled for read\n", arg2);
                return -1;
            }
            if (fs_impl->read_impl == NULL) {
                kprintf("FD # %d does not support reading.\n", arg2);
                return -1;
            }
            ssize_t len_read = fs_impl->read_impl((void *) arg3, arg4, fo->content_start, fo->offset);
            if (len_read > 0) {
                fo->offset += len_read;
            }
            return len_read;
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
            file_object_t *fo = (file_object_t *) kmalloc(sizeof(file_object_t));
            fo->file_path = (char *) arg2;
            fo->offset = 0;
            fo->ref_count = 1;
            fo->file_sys_impl = get_tarfs_impl();
            fo->content_start = file;
            cur_kt->fds[fd_idx] = fo;
            return fd_idx;
            // do later
        }
        case SYSCALL_CLOSE: {
            if (arg2 < 0 || arg2 >= NUM_FDS) {
                // fd limit reached
                kprintf("FD # is incorrect!\n");
                return -1;
            }
            if (arg2 == 0 || arg2 == 1 || arg2 == 2) {
                kprintf("Can't close FD #%d\n", arg2);
                return -1;
            }

            // now, make new file object
            kthread_t *cur_kt = get_current_process();
            file_object_t *fo = cur_kt->fds[arg2];
            if (fo == NULL) {
                kprintf("FD #%d is not open\n", arg2);
                return -1;
            }
            fo->ref_count -= 1;
            if (fo->ref_count == 0) {
                cur_kt->fds[arg2] = NULL;
                kfree(fo);
            }
            return 0;
        }

        case SYSCALL_YIELD:
            scheduler();
            return 0;
        case SYSCALL_PID:
            return get_process_pid();
        case SYSCALL_PPID:
            return get_process_ppid();
        case SYSCALL_MALLOC:
            return user_malloc(arg2);
        case SYSCALL_FREE:
            user_free(arg2);
            return 0;
        case SYSCALL_OPENDIR:
            return open_dir((char *) arg2);
        case SYSCALL_READDIR:
            return read_dir(arg2, (char *) arg3);
        case SYSCALL_CLOSEDIR:
            return close_dir(arg2);
        case SYSCALL_GETCWD:
            return get_cwd((char *) arg2, arg3);
        case SYSCALL_CHDIR:
            return ch_dir((char *) arg2);
        case SYSCALL_SLEEP:
            return (uint64_t) sleep((unsigned int) arg2);
        case SYSCALL_PS:
            get_process_state((char *) arg2);
            break;
        default:
            kprintf("Arg1: %x, Arg2: %x, Arg3: %x \n", code, arg2, arg3);
            kprintf("Arg4: %x, Arg5: %x, Arg6: %x \n", arg4, arg5, arg6);
    }
    return 0;
}