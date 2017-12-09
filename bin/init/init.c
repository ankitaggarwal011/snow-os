#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define STDIN 0
#define STDOUT 1
#define STDERR 2

#define BUF_SIZE 256
#define MAX_ARGS 8
#define MAX_LINES 256

char *remove_white_spaces(char *src) {
    while (*src == ' ') {
        src++;
    }
    int i = strlen(src) - 1;
    while (src[i] == ' ') {
        src[i] = 0;
        i--;
    }
    return src;
}

int parse_split(char *commands[], char *input, const char delimiter) {
    int i = 0, last = -1, n = 0;
    while (input[i] != 0) {
        if (input[i] == delimiter) {
            input[i] = 0;
            commands[n] = &input[last + 1];
            commands[n] = remove_white_spaces(commands[n]);
            last = i;
            n++;
        }
        i++;
    }
    commands[n] = &input[last + 1];
    commands[n] = remove_white_spaces(commands[n]);
    commands[++n] = 0;
    return n;
}

int launch_process(char *process_name, char *envp[]) {
    char *arguments[MAX_ARGS];
    arguments[0] = process_name;
    arguments[1] = 0;
    pid_t c_pid = fork();
    if (c_pid == 0) {
        execvpe(process_name, arguments, envp);
        char *error_msg = "Command not found\n";
        write(STDOUT, error_msg, strlen(error_msg));
        exit(0);
    }
    else {
        while(1) yield();
    }
    return 0;
}


int shell_execfile(char *filename, char *envp[]) {
    if (filename[0] == '/') filename++;
    int fp = open(filename, 0);
    if (fp < 0) {
        char *err_msg = "File not found\n";
        write(1, err_msg, strlen(err_msg));
        return 0;
    }
    char script_file[BUF_SIZE * 4];
    read(fp, script_file, BUF_SIZE);
    char *line_read[MAX_LINES];
    int num_lines = parse_split(line_read, script_file, '\n');
    for (int i = 0; i < num_lines; i++) {
        if (line_read[i][0] != '#' && line_read[i][1] != '!' && strlen(line_read[i]) != 0) {
            launch_process(line_read[i], envp);
        }
    }
    close(fp);
    return 0;
}

int main(int argc, char *argv[], char *envp[]) {
    shell_execfile("/etc/rc", envp);
    return 0;
}
