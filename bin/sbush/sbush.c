#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[], char *envp[]) {
    char *welcome = "Welcome to SBUnix!\n";
    char *a = "hiwhattefun";
    read(0, a, 5);
    yield();
    write(1, welcome, 19);
    yield();
    write(1, a, 5);
    yield();
    write(1, welcome, 19);

    /*
    if (fork() == 0) {
      write(1, "Parent\n", 7);
      yield();
      write(1, "Parent is back\n", 15);
    }
    else {
      write(1, "Child\n", 6);
    }
    */
//    yield();
//    char buf[10];
//    ssize_t len = read(0, buf, 5);
//    write(1, buf, len);
//    while (1) {
//        yield();
//        write(1, welcome, 19);
//    }
    while (1);
    return 0;
}
