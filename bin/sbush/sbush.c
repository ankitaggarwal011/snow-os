#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[], char *envp[]) {
    write(1, " start ", 7);
    int fork_res = fork();
    if (fork_res == 0) {
        write(1, " child ", 7);
        yield();
    } else {
        write(1, " parent1 ", 9);
        yield();
        write(1, " parent2 ", 9);
        yield();
    }
    write(1, " end ", 5);
    while (1);
    return 0;
}
