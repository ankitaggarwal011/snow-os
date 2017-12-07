#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>

int main(int argc, char *argv[], char *envp[]) {
    char a[10];
    a[0] = 'x';
    a[1] = 'y';
//    int len = read(0, a, 5);
    write(2, a, 2);
    a[2] = 'z';
    write(2, a, 3);
//    int len2 = read(0, a, len);
//    write(1, a, len2);
    while (1);
    return 0;
}
