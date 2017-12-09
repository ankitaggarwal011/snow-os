#ifndef TERMINAL_H
#define TERMINAL_H

#include <sys/defs.h>
#include <sys/vfs.h>

#define TERM_BUF_SIZE 256

file_sys_impl_t *get_terminal_fs_impl();

void on_new_char_recvd(const char *ctr);

#endif
