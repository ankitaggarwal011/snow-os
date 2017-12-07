#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[], char *envp[]) {
    write(1, "start ", 6);
    char n[5];
    itoa_cl(n, getpid());
    write(1, n, strlen(n));
    write(1, "\n frk  ", 7);
    int fork_res = fork();
    itoa_cl(n, fork_res);
    write(1, n, strlen(n));
    if (fork_res == 0) {
        write(1, "child  ", 7);
    } else {
        write(1, "parent1 ", 8);
        yield();

        char n[5];
        write(1, "parent2 ", 8);
        itoa_cl(n, getpid());
        write(1, n, strlen(n));
        write(1, " ", 1);
        itoa_cl(n, getppid());
        write(1, n, strlen(n));
        write(1, " ", 1);
    }
    write(1, "end", 3);
    while (1);
    return 0;
}
