#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<dirent.h>
#include<string.h>

#define O_DIRECTORY	00200000
#define BUF_SIZE 1024
#define SYS_GEDDENTS 78

struct linux_dirent {
   unsigned short d_reclen;
   char d_name[];
};

long cus_readdir(long fd, char* buffer, long size) {
    long return_val;
    __asm__ volatile
    (
        "syscall"
        : "=a" (return_val)
        : "0"(SYS_GEDDENTS), "D"(fd), "S"(buffer), "d"(size)
        : "cc", "r11", "rcx", "memory"
    );
    return return_val;
}

int list_directory(char* location) {
    struct linux_dirent* d;
    int fd = open(location, O_RDONLY | O_DIRECTORY);
    int items = 0;
    char buffer[BUF_SIZE];
    if (fd != -1) {
        items = cus_readdir(fd, buffer, BUF_SIZE);
    }
    else {
        char* temp_var = "ls: No such file or directory";
        if (write(1, temp_var, sizeof(char) * strlen(temp_var)) == -1) { /* Write error */ }
    }
    for (int i = 0; i < items;) {
        d = (struct linux_dirent*) (buffer + i);
        char* temp_var_1 = d->d_name;
        strcat(temp_var_1, "\n");
        if (write(1, temp_var_1, sizeof(char) * strlen(temp_var_1)) == -1) { /* Write error */ }
        i += d->d_reclen;
    }
    close(fd);
    return 0;
}

int main(int argc, char *argv[], char *envp[]) {
    if (argc == 1) {
        list_directory(".");
    }
    else if (argc == 2) {
        list_directory(argv[1]);
    }
    else {
        char* temp_var = "Incorrect number of command line arguments. Please try again.";
        if (write(1, temp_var, sizeof(char) * strlen(temp_var)) == -1) { /* Write error */ }
    }
    return 0;
}
