#ifndef _TARFS_H
#define _TARFS_H

#include <sys/vfs.h>
extern char _binary_tarfs_start;
extern char _binary_tarfs_end;

struct posix_header_ustar {
    char name[100];
    char mode[8];
    char uid[8];
    char gid[8];
    char size[12];
    char mtime[12];
    char checksum[8];
    char typeflag[1];
    char linkname[100];
    char magic[6];
    char version[2];
    char uname[32];
    char gname[32];
    char devmajor[8];
    char devminor[8];
    char prefix[155];
    char pad[12];
};

struct dir_header {
    int current_point;
    int file_count;
    char files[100][256]; // can have 1000 files for now
    int dir_exists;
}__attribute__((__packed__));

void init_tarfs();

void print_all_files();

void *get_file(char *filename);

void *get_file_binary(char *filename);

//hack
ssize_t tarfs_read(void *buffer, int len, char *file, int offset);

file_sys_impl_t *get_tarfs_impl();
uint64_t open_dir(char *name);
int read_dir(uint64_t stream, char *filename);
int close_dir(uint64_t stream);

struct dir_header *get_folder(char *name);
int substr(char *s1, char *s2);

#endif
