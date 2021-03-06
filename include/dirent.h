#ifndef _DIRENT_H
#define _DIRENT_H

#include <unistd.h>

#define NAME_MAX 255

struct dirent {
 char d_name[NAME_MAX+1];
}__attribute__((__packed__));

struct DIR {
    uint64_t stream;
}__attribute__((__packed__));

typedef struct DIR DIR;

DIR *opendir(const char *name);
struct dirent *readdir(DIR *dirp);
int closedir(DIR *dirp);

#endif
