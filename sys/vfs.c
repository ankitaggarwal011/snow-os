#include <sys/vfs.h>
#include <sys/defs.h>
#include <sys/terminal.h>
#include <sys/paging.h>
file_object_t *create_terminal_fo(char *name);

file_sys_impl_t *terminal_fs;
file_object_t *stdin_fo;
file_object_t *stdout_fo;

file_object_t *get_stdin_fo() {
    if (stdin_fo == NULL) {
        stdin_fo = create_terminal_fo("stdin");
    }
    return stdin_fo;
}

file_object_t *get_stdout_fo() {
    if (stdout_fo == NULL) {
        stdout_fo = create_terminal_fo("stdout");
    }
    return stdout_fo;
}

file_object_t *create_terminal_fo(char *name) {
    file_object_t *fo = (file_object_t *) kmalloc(sizeof(file_object_t));
    fo->file_path = name;
    if (terminal_fs == NULL) {
        terminal_fs = get_terminal_fs_impl();
    }
    fo->file_sys_impl = terminal_fs;
    fo->ref_count = 0;
    return fo;
}