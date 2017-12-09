#include <stdio.h>
#include <unistd.h>

char *gets(char *s) {
    read(0, s, 256);
    return s;
}