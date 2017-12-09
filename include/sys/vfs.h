//
// Created by Saraj Munjal on 11/30/17.
//

#ifndef SNOW_OS_VFS_H
#define SNOW_OS_VFS_H

#include <sys/defs.h>

typedef ssize_t (*write_fp)(void *buffer, int len, char *file, int offset);

typedef ssize_t (*read_fp)(void *buffer, int len, char *file, int offset);

typedef struct file_sys_impl {
    // read contract: read() // not clear
    read_fp read_impl;
    // write contract: not clear() //
    write_fp write_impl;
} file_sys_impl_t;
typedef struct file_obj {
    char *file_path;
    file_sys_impl_t *file_sys_impl;
    uint16_t ref_count;
    char *content_start;
    uint32_t offset;
} file_object_t;

file_object_t *get_stdin_fo();

file_object_t *get_stdout_fo();

#endif //SNOW_OS_VFS_H
