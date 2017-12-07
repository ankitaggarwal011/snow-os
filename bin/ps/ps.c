#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[], char *envp[]) {
    int MAX_PROC = 20;
    char buf[MAX_PROC * 128];

//    for (int i = 1; i < MAX_PROC; i++) {
////        buf[i] = (char *) malloc(sizeof(char) * 128);
//        buf[i][0] = '\0';
////        strcat(buf[i], "Empty");
////        buf[i][5] = '\0';
//    }
    char *msg = "\n ------------------ Process State  ------------------ \n";
    write(1, msg, strlen(msg));
    get_process_state(buf);
//    for (int i = 1; i < MAX_PROC; i++) {
    write(1, (char *) buf, strlen((char *) buf));

    char *end_msg = "\n ------------------ End Process State  ------------------ \n";
    write(1, end_msg, strlen(end_msg));
    get_process_state(buf);
//        write(1, "\n", 1);
//    }
    while (1);
    return 0;
}