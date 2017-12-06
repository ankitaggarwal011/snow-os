#include <sys/defs.h>
#include <sys/kprintf.h>
#include <sys/physical_memory.h>
#include <sys/string.h>
#include <sys/tarfs.h>
#include <sys/vfs.h>
#include <sys/paging.h>

struct posix_header_ustar *tarfs_start = (struct posix_header_ustar *) &_binary_tarfs_start;
struct posix_header_ustar *tarfs_end = (struct posix_header_ustar *) &_binary_tarfs_end;

void print_all_files() {
    struct posix_header_ustar *s = tarfs_start;
    kprintf("Printing all available files in the tarfs:\n");
    do {
        if (!s || s->name[0] == '\0') {
            break;
        }
        int file_size = o_to_d(atoi(s->size));

        kprintf("Name: %s, Size: %d bytes, Type: %s\n", s->name, file_size, s->typeflag);

        if (file_size > 0) {
            s += (file_size / sizeof(struct posix_header_ustar)) + 2;
        } else {
            s++;
        }
    } while (s < tarfs_end);
}

void *get_file(char *filename) {
    struct posix_header_ustar *s = tarfs_start;
    do {
        if (!s || s->name[0] == '\0') {
            break;
        }
        int file_size = o_to_d(atoi(s->size));
        char *file = (char *) (s + 1);

        if (kstrcmp(filename, s->name) == 0) {
            kprintf("File found: Name: %s, Size: %d bytes, Type: %s\n", s->name, file_size, s->typeflag);
            return (void *) file;
        }

        if (file_size > 0) {
            s += (file_size / sizeof(struct posix_header_ustar)) + 2;
        } else {
            s++;
        }

    } while (s < tarfs_end);

    kprintf("File not found.\n");
    return NULL;
}

void *get_file_binary(char *filename) {
    struct posix_header_ustar *s = tarfs_start;
    kprintf("Printing all available files in the tarfs:\n");
    do {
        if (!s || s->name[0] == '\0') {
            break;
        }
        int file_size = o_to_d(atoi(s->size));
        char *file = (char *) (s + 1);

        if (kstrcmp(filename, s->name) == 0 && atoi(s->typeflag) == 0) {
            kprintf("File found: Name: %s, Size: %d bytes, Type: %s\n", s->name, file_size, s->typeflag);
            return (void *) file;
        }

        if (file_size > 0) {
            s += (file_size / sizeof(struct posix_header_ustar)) + 2;
        } else {
            s++;
        }

    } while (s < tarfs_end);

    kprintf("File not found.\n");
    return NULL;
}

void init_tarfs() {
    print_all_files();
}

ssize_t tarfs_read(void *buffer, int len, char *file, int offset) {
    char *c = (char *) file;
    int i = 0;
    char *buf = (char *) buffer;
    while (i < len) {
        if (*(c + offset + i) == '\0') {
            return i;
        }
        buf[i] = *(c + offset + i);
        i++;
    }
    return len;
}

file_sys_impl_t *tarfs_impl;

file_sys_impl_t *get_tarfs_impl() {
    if (tarfs_impl == NULL) {
        tarfs_impl = (file_sys_impl_t *) kmalloc(sizeof(file_sys_impl_t));
        tarfs_impl->read_impl = tarfs_read;
        tarfs_impl->write_impl = NULL;
    }
    return tarfs_impl;
}