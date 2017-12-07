#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>

int main(int argc, char *argv[], char *envp[]) {
    char *args[] = {"bin/cat", "etc/test.txt"};
    execvpe("bin/cat", args, NULL);
    while (1);
    return 0;
}
