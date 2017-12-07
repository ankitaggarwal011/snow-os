#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>

int main(int argc, char *argv[], char *envp[]) {
    char *a = "hellooo";
    volatile int len = read(0, a, 5);
    write(2, a, len);
    int len2 = read(0, a, len);
    write(1, a, len2);
    while (1);
    return 0;
}
