#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[], char *envp[]) {
//    int MAX_PROC = 20;
//    char buf[MAX_PROC][512];

//    for (int i = 1; i < MAX_PROC; i++) {
//        strcat(buf[i], "Empty");
//        buf[i][5] = '\0';
//    }
    write(1, "poop", 4);
//    get_process_state((char **) buf);
//    for (int i = 1; i < MAX_PROC; i++) {
//        write(1, buf[i], strlen(buf[i]));
//        write(1, "\n", 1);
//    }
    while (1);
    return 0;
}