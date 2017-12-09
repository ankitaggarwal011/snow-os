#include <unistd.h>
#include <signal.h>
#include <string.h>

int main(int argc, char *argv[], char *envp[]) {
    if (argc < 3) {
        char *err_msg = "Kill -9 must have at least 1 argument\n";
        write(1, err_msg, strlen(err_msg));
        return 0;
    }
    int vals[argc - 2];
    for (int i = 2; i < argc; i++) {
        uint32_t val = atoi(argv[i]);
        if (val == -1) {
            char *err_msg = "Arguments for kill -9 are not correct positive numbers\n";
            write(1, err_msg, strlen(err_msg));
            return 0;
        }
        vals[i - 1] = val;
    }
    for (int i = 0; i < argc - 1; i++) {
        kill(vals[i], 0);
    }
    return 0;
}
