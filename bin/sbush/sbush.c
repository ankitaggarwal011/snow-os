#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>

int main(int argc, char *argv[], char *envp[]) {
    char *a = "hello";
    read(0, a, 5);
    write(1, a, 5);
    while (1);
    return 0;
}
