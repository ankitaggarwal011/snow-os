#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[], char *envp[]) {
    write(1, " start ", 7);
    int fork_res = fork();
    if (fork_res == 0) {
        char *argv[] = {"bin/ps"};
        execvpe("bin/ps", argv, NULL);
    } else {
        write(1, " parent1 ", 9);
        yield();
        write(1, " parent2 ", 9);
        yield();
    }
    write(1, " end ", 5);
    int fork2 = fork();
    if (fork2 == 0) {
        char *argv[] = {"bin/ps"};
        execvpe("bin/ps", argv, NULL);
    } else {
        write(1, " ps2 ", 5);
        yield();
        write(1, " psfin ", 7);
        yield();
    }
    while (1);
    return 0;
}