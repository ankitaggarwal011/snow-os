//
// Created by Saraj Munjal on 12/1/17.
//

#ifndef SNOW_OS_TERMINAL_H
#define SNOW_OS_TERMINAL_H

#include <sys/defs.h>
#include <sys/vfs.h>

#define TERM_BUF_SIZE 1024

file_sys_impl_t *get_terminal_fs_impl();

void on_new_char_recvd(const char *ctr);

#endif //SNOW_OS_TERMINAL_H
