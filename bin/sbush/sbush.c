#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>

int main(int argc, char *argv[], char *envp[]) {
    // char *welcome = "Welcome to SBUnix!\n";
    // write(1, welcome, 19);
    /*
    char *a = "hiwhattefun";
    read(0, a, 5);
    yield();
    write(1, welcome, 19);
    yield();
    write(1, a, 5);
    yield();
    */
    /*
    char *test_malloc = (char *) malloc(10);
    free(test_malloc);
    */
    /*
    char *a = "hello";
    read(0, a, 5);
    write(0, "got: ", 5);
    write(0, a, 5);
    write(0, "\n", 1);
    */
    
    if (fork() != 0) {
      write(1, "Parent\n", 7);
      yield();
      write(1, "Parent is back\n", 15);
    }
    else {
      write(1, "Child\n", 6);
    }
    
//    yield();
//    char buf[10];
//    ssize_t len = read(0, buf, 5);
//    write(1, buf, len);
//    while (1) {
//        yield();
//        write(1, welcome, 19);
//    }
    /*
    DIR *test = opendir("bin");
    struct dirent *test_dir;
    while(1) {
        test_dir = readdir(test);
        //write(1, (char *) test_dir->d_name, 256);
        if (test_dir == NULL) break;
    }
    closedir(test);
    */
    /*
    char buff[256];
    getcwd((char *) buff, 256);
    chdir("bin/test");
    getcwd((char *) buff, 256);
    */
    while (1);
    return 0;
}
