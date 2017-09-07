/* Author: Ankit Aggarwal */

#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include<dirent.h>

#define BUF_SIZE 1024

int list_directory(char* location) {
    DIR *directory;
    struct dirent *d;
    if ((directory = opendir(location)) != NULL) {
        while ((d = readdir(directory)) != NULL) {
            puts(d->d_name);
        }
    }
    else {
        puts("ls: No such file or directory");
    }
    closedir(directory);
    return 0;
}

int main(int argc, char *argv[], char *envp[]) {
    if (argc == 1) {
        char* current_dir;
        current_dir = (char*) malloc(sizeof(char) * BUF_SIZE);
        current_dir = getcwd(current_dir, BUF_SIZE);
        list_directory(current_dir);
    }
    else if (argc == 2) {
        list_directory(argv[1]);
    }
    else {
        puts("Incorrect number of command line arguments. Please try again.");
    }
    return 0;
}
