#include<unistd.h>
#include<string.h>

#define BUF_SIZE 1024

int main(int argc, char *argv[], char *envp[]) {
    if (argc == 2) {
        int fd = open(argv[1], 0);
        if (fd < 0) {
            char *err_msg = "File not found\n";
            write(1, err_msg, strlen(err_msg));
            return 0;
        }
        char buf[BUF_SIZE];
        while (1) {
            ssize_t len_read = read(fd, buf, BUF_SIZE);
            if (len_read == -1) {
                char *err_msg = "Failed to read from file\n";
                write(1, err_msg, strlen(err_msg));
                break;
            }
            write(1, buf, len_read);
            if (len_read < BUF_SIZE) {
                break;
            }
        }
        write(1, "\n", 1);
        close(fd);
    }
    else {
        char *err_msg = "Incorrect number of command line arguments. Please try again.\n";
        write(1, err_msg, strlen(err_msg));
    }
    return 0;
}