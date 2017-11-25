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

int kstrcmp(char *s1, char *s2) {
    while (*s1 && *s2 && *s1 == *s2) {
        s1++;
        s2++;
    }
    if (*s1 == 0 && *s2 == 0) {
        return 0;
    }
    else if (*s1 > *s2) {
        return 1;
    }
    return -1;
}

