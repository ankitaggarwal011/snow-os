#include<unistd.h>
#include<string.h>

#define BUF_SIZE 1000

int main(int argc, char *argv[], char *envp[]) {
    if (argc == 2) {
        int fd = open(argv[1], 0);
        char buf[BUF_SIZE];
        while (1) {
            ssize_t len_read = read(fd, buf, BUF_SIZE);
            if (len_read == -1) {
                char *err_msg = "Failed to read from file\n";
                write(1, "Failed to read from file\n", strlen(err_msg));
                break;
            }
            write(1, buf, len_read);
            if (len_read < BUF_SIZE) {
                break;
            }
        }
        close(fd);
    } else {
        char *temp_var = "Incorrect number of command line arguments. Please try again.\n";
        if (write(1, (char *) temp_var, sizeof(char) * strlen(temp_var)) == -1) { /* Write error */ }
    }
    while (1);
    return 0;
}