#include <stdio.h>
#include <string.h>

char *gets(char *s) {
    read(0, s, 256);
    return s;
}