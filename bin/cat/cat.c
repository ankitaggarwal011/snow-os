#include<unistd.h>
#include<string.h>
#include <fcntl.h>

// #define O_RDONLY 0x0000
#define BUF_SIZE 1024000

int main(int argc, char *argv[], char *envp[]) {
    if (argc == 2) {
        int fp = open(argv[1], O_RDONLY);
        char script_file[BUF_SIZE];
        if (read(fp, (char*) script_file, BUF_SIZE) != -1) {
            if (write(1, script_file, sizeof(char) * strlen(script_file)) == -1) { /* Write error */ }
        }
    }
    else {
        char temp_var[] = "Incorrect number of command line arguments. Please try again.";
        if (write(1, (char*)temp_var, BUF_SIZE) == -1) { /* Write error */ }
    }
    return 0;
}
