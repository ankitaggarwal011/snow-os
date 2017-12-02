#include <sys/terminal.h>
#include <sys/vfs.h>
#include <sys/defs.h>
#include <sys/kprintf.h>

ssize_t terminal_read(void *buffer, int len);

ssize_t terminal_write(void *buffer, int len);

file_sys_impl_t terminal_fs_impl = {
        terminal_read,
        terminal_write
};

file_sys_impl_t *get_terminal_fs_impl() {
    return &terminal_fs_impl;
}

char term_buf[TERM_BUF_SIZE];
uint16_t index;


void reset_term_inp_buffer() {
    memset(term_buf, 0, TERM_BUF_SIZE);
    index = 0;
}


ssize_t terminal_read(void *buffer, int len) {
    if (len == 0) {
        return 0;
    }
    reset_term_inp_buffer();
    while (index < len - 1);
    // index must be >=len-1
    kprintf("Buffer complete: ");
    for (int i = 0; i < index; i++) {
        kprintf("%c", term_buf[i]);
    }
    kprintf("\n");
    return index;
}

ssize_t terminal_write(void *buffer, int len) {
    if (len <= 0) {
        return 0;
    }
    char *str = (char *) buffer;
    for (int i = 0; i < len; i++) {
        kprintf("%c", str[i]);
    }
    return len;
}


void on_new_char_recvd(const char *ctrs) {
    //advanced use case
    /*if (index == -1) {
        // start new buffer
        reset_term_inp_buffer();

    }*/
    int i = 0;
    while (ctrs[i] != '\0') {
        term_buf[index++] = ctrs[i++];
    }
}