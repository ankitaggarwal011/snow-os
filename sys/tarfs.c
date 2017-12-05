#include <sys/defs.h>
#include <sys/kprintf.h>
#include <sys/physical_memory.h>
#include <sys/string.h>
#include <sys/tarfs.h>
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

ssize_t tarfs_read(void *buffer, int len, void *file, int offset) {
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

struct dir_header *get_folder(char *name) {
    struct dir_header *dir_pointer = (struct dir_header *) kmalloc(sizeof(struct dir_header));
    dir_pointer->file_count = 0;
    struct posix_header_ustar *s = tarfs_start;
    do {
        if (!s || s->name[0] == '\0') {
            break;
        }
        int file_size = o_to_d(atoi(s->size));

        if (substr(name, s->name)) {
            char *tmp = s->name;
            int j = 0;
            while(tmp) {
                dir_pointer->files[dir_pointer->file_count][j] = tmp;
                tmp++;
                j++;
            }
            tmp[j] = 0;
            dir_pointer->file_count++;
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
    return (uint64_t) get_folder(name);
}

int read_dir(uint64_t stream, char* filename) {
    struct dir_header *d = (struct dir_header *) stream;
    if (d->current_point >= dir_pointer->file_count) {
        return -1;
    }
    struct dir_header *d = (struct dir_header *) stream;
    for (int i = 0; i < 255; i++) {
        *(filename + i) = d->files[current_point][i];
    }
    d->current_point++;
    return 0;
}

int close_dir(uint64_t stream) {
    kfree((void *) stream);
    return 0;
}