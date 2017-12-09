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
            // kprintf("File found: Name: %s, Size: %d bytes, Type: %s\n", s->name, file_size, s->typeflag);
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
    do {
        if (!s || s->name[0] == '\0') {
            break;
        }
        int file_size = o_to_d(atoi(s->size));
        char *file = (char *) (s + 1);

        if (kstrcmp(filename, s->name) == 0 && atoi(s->typeflag) == 0) {
            // kprintf("File found: Name: %s, Size: %d bytes, Type: %s\n", s->name, file_size, s->typeflag);
            return (void *) file;
        }

        if (file_size > 0) {
            s += (file_size / sizeof(struct posix_header_ustar)) + 2;
        } else {
            s++;
        }

    } while (s < tarfs_end);

    // kprintf("File not found.\n");
    return NULL;
}

int dir_exists(char *filename) {
    struct posix_header_ustar *s = tarfs_start;
    do {
        if (!s || s->name[0] == '\0') {
            break;
        }
        int file_size = o_to_d(atoi(s->size));
        char *file = (char *) (s + 1);

        if (kstrcmp(filename, s->name) == 0 && atoi(s->typeflag) == 5) {
            // kprintf("File found: Name: %s, Size: %d bytes, Type: %s\n", s->name, file_size, s->typeflag);
            return 0;
        }

        if (file_size > 0) {
            s += (file_size / sizeof(struct posix_header_ustar)) + 2;
        } else {
            s++;
        }

    } while (s < tarfs_end);
    return -1; 
}

void init_tarfs() {
    print_all_files();
}

ssize_t tarfs_read(void *buffer, int len, char *file, int offset) {
    char *c = (char *) file;
    int i = 0;
    char *buf = (char *) buffer;
    while (i < len) {
        if (*(c + offset + i) == '\0' || *(c + offset + i) == EOF) {
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

struct dir_header *get_folder(char *name) {
    struct dir_header *dir_pointer = (struct dir_header *) kmalloc(sizeof(struct dir_header));
    dir_pointer->file_count = 0;
    dir_pointer->dir_exists = 0;
    struct posix_header_ustar *s = tarfs_start;
    do {
        if (!s || s->name[0] == '\0') {
            break;
        }
        int file_size = o_to_d(atoi(s->size));

        // check to exclude the directory itself
        char tmp_copy[256];
        char *tmp_1 = (char *) name, *tmp_2 = (char *) tmp_copy;
        while (*tmp_1) {
            *tmp_2 = *tmp_1;
            tmp_1++;
            tmp_2++;
        }
        *tmp_2 = '/';
        tmp_2++;
        *tmp_2 = '\0';

        if (substr(name, s->name) && kstrcmp((char *) tmp_copy, s->name) != 0) {
            char *tmp = s->name;
            int j = 0;
            while (*tmp) {
                dir_pointer->files[dir_pointer->file_count][j] = *tmp;
                tmp++;
                j++;
            }
            dir_pointer->files[dir_pointer->file_count][j] = 0;
            dir_pointer->file_count++;
            dir_pointer->dir_exists = 1;
        }
        else if (kstrcmp((char *) tmp_copy, s->name) == 0){
            dir_pointer->dir_exists = 1;
        }

        if (file_size > 0) {
            s += (file_size / sizeof(struct posix_header_ustar)) + 2;
        } else {
            s++;
        }
    } while (s < tarfs_end);
    return dir_pointer;
}

int substr(char *s1, char *s2) {
    char *s1_ = s1, *s2_ = s2;
    while (*s1_ && *s2_) {
        if (*s1_ != *s2_) {
            return 0;
        }
        s1_++;
        s2_++;
    }
    return 1;
}

uint64_t open_dir(char *name) {
    struct dir_header *dir = get_folder(name);
    if (dir->dir_exists == 1) {
        return (uint64_t) dir;
    }
    return 0;
}

int read_dir(uint64_t stream, char *filename) {
    struct dir_header *d = (struct dir_header *) stream;
    if (d->current_point >= d->file_count) {
        return -1;
    }
    for (int i = 0; i < BUF_SIZE; i++) {
        *(filename + i) = d->files[d->current_point][i];
    }
    d->current_point++;
    return 0;
}

int close_dir(uint64_t stream) {
    kfree((void *) stream);
    return 0;
}