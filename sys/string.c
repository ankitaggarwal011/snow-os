//
// Created by Saraj Munjal on 9/18/17.
//

#include <sys/string.h>

char* strcat(char *s1, const char *s2) {
    char *s = s1;
    while (*s != '\0') {
        s++;
    }
    const char *t = s2;
    while (*t != '\0') {
        *s = *t;
        s++;
        t++;
    }
    return s1;
}

