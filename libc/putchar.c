#include <stdio.h>
#include <unistd.h>

int putchar(int c) {
    char w[1];
    w[0] = c;
    return write(1, w, 1);
}
