#include <sys/terminal.h>
#include <sys/vfs.h>
#include <sys/defs.h>
#include <sys/kprintf.h>

ssize_t terminal_read(void *buffer, int len, char *content, int offset);

ssize_t terminal_write(void *buffer, int len, char *content, int offset);

file_sys_impl_t terminal_fs_impl = {
        terminal_read,
        terminal_write
};

file_sys_impl_t *get_terminal_fs_impl() {
    return &terminal_fs_impl;
}

char term_buf[TERM_BUF_SIZE];
volatile uint16_t index;

void reset_term_inp_buffer() {
    memset(term_buf, 0, TERM_BUF_SIZE);
    index = 0;
}


ssize_t terminal_read(void *buffer, int len, char *file, int offset) {
    if (len == 0) {
        return 0;
    }
    char *buf = (char *) buffer;
    // check for already buffered input
//    if (index >= len) {
//        int i = 0;
//        while (i < len) {
//            buf[i] = term_buf[i];
//            i++;
//        }
//        int j = i - len;
//        while (i <= index) {
//            term_buf[j] = term_buf[i];
//            i++;
//            j++;
//        }
//        while (j < TERM_BUF_SIZE) {
//            term_buf[j] = '\0';
//            j++;
//        }
//        return len;
//    }
    reset_term_inp_buffer();
    int last_index = index;
    while (1) {
        if (last_index == index) {
            continue;
        }
        last_index = index;
        if (term_buf[index - 1] == '\n') {
            int cpy_val = index - 1;
            reset_term_inp_buffer();
            return cpy_val;
        }
        buf[index - 1] = term_buf[index - 1];
        if (index == len ) {
            reset_term_inp_buffer();
            return len;
        }

    }
    // index must be >=len-1

}

ssize_t terminal_write(void *buffer, int len, char *content, int offset) {
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
    // main character
    if (ctrs[1] == 'H' && ctrs[0] == '^') {
        // backspace!!
        if (index == 0) {
            return;
        }
        term_buf[index--] = '\0';
        handle_video_mem_backspace();
    } else if (ctrs[1] == 'r' && ctrs[0] == '\\') {
        term_buf[index++] = '\n';
        kprintf("\n");
    } else {
        term_buf[index++] = ctrs[0];
        kprintf("%c", ctrs[0]);
    }
}