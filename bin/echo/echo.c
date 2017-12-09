#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[], char *envp[]) {
    if (argc < 2) {
        return 0;
    }
    for (int i = 1; i < argc; i++) {
        char *s = argv[i];
        write(1, s, strlen(s));
        write(1, " ", 1);
    }
    write(1, "\n", 1);
    return 0;
}