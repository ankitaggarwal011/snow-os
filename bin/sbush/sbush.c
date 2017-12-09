#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define STDIN 0
#define STDOUT 1
#define STDERR 2

#define BUF_SIZE 128
#define MAX_ARGS 8

struct job {
    char jobname[BUF_SIZE];
    char *arguments[MAX_ARGS];
    int num_args;
};

int is_background;
char PS1[BUF_SIZE] = "cwd: sbush> ";
int PS1_set = 0;
char path[BUF_SIZE] = "bin/";

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
    return ++n;
}

int put_env(char *new_path, char *envp[]) {
    int i = 5;
    while(new_path[i]) {
        path[i - 5] = new_path[i];
        i++;
    }
    while(path[i - 5]) path[i - 5] = 0;
    return 0;
}

int exec_binary(char *arguments[], int num_args, char *envp[]) {
    char bin[BUF_SIZE];
    int i = 0;
    for (i = 0; (path[i] != '/' && path[i] != '\0'); i++) {
        bin[i] = path[i];
    }
    bin[i++] = '/';
    bin[i] = '\0';
    strcat(bin, arguments[0]);
    pid_t c_pid = fork();
    if (c_pid == 0) {
        execvpe(bin, arguments, envp);
        char *error_msg = "Command not found\n";
        write(STDOUT, error_msg, strlen(error_msg));
        exit(0);
    }
    else {
        if (is_background == 0) {
            // waitpid(c_pid, NULL);
        }
        yield();
    }
    return 0;
}

int set_env_variables(char *arguments[], int num_args, char *envp[]) {
    if (arguments[1][0] == 'P' && arguments[1][1] == 'S' && arguments[1][2] == '1') {
        char *tokens[MAX_ARGS];
        parse_split(tokens, arguments[1], '=');
        int k = 0;
        while(tokens[1][k]) {
            PS1[k] = tokens[1][k];
            k++;
        }
        while(PS1[k]) PS1[k] = 0;
        for (int i = 2; i < num_args; i++) {
            strcat(PS1, " ");
            strcat(PS1, arguments[i]);
        }
        strcat(PS1, " ");
        PS1_set = 1;
    }
    else {
        int status = put_env(arguments[1], envp);
        if (status != 0) {
            char *error_msg = "export: Error setting environment variable\n";
            write(STDOUT, error_msg, strlen(error_msg));
        }
    }
    return 0;
}

int cd(char *arguments[], int num_args, char *envp[]) {
    int status;
    if (num_args == 2) {
        if (arguments[1][0] == '/') {
            status = chdir(arguments[1]);
        }
        else {
            char cdir[BUF_SIZE];
            getcwd(cdir, BUF_SIZE);
            strcat(cdir, "/");
            strcat(cdir, arguments[1]);
            status = chdir(cdir);
        }
        if (status != 0) {
            char *error_msg = "cd: No such file or directory\n";
            write(STDIN, error_msg, strlen(error_msg));
        }
    }
    else {
        char *error_msg = "cd: Incorrect number of arguments\n";
        write(STDIN, error_msg, strlen(error_msg));
    }
    return 0;
}

int shell_parse(char *input, int len_input, char *envp[]) {    
    if (strcmp(input, "$PATH") == 0) {
        write(STDOUT, path, strlen(path));
        write(STDOUT, "\n", 1);
        return 0;
    }
    else if (strcmp(input, "$PS1") == 0) {
        write(STDOUT, PS1, strlen(PS1));
        write(STDOUT, "\n", 1);
        return 0;
    }

    if (input[len_input - 1] == '&') {
        is_background = 1;
        input[len_input - 1] = 0;
        len_input--;
    }

    input = remove_white_spaces(input);
    struct job shell_job;
    shell_job.num_args = parse_split(shell_job.arguments, input, ' ');

    char* jobname = shell_job.arguments[0];
    if (strcmp(jobname, "cd") == 0) {
        cd(shell_job.arguments, shell_job.num_args, envp);
    }
    else if (strcmp(jobname, "export") == 0) {
        set_env_variables(shell_job.arguments, shell_job.num_args, envp);
    }
    else {
        exec_binary(shell_job.arguments, shell_job.num_args, envp);
    }

    return 0;
}

int shell_execfile(char *filename, char *envp[]) {
    char bin[BUF_SIZE];
    int i = 0;
    for (i = 0; (path[i] != '/' && path[i] != '\0'); i++) {
        bin[i] = path[i];
    }
    bin[i++] = '/';
    bin[i] = '\0';
    strcat(bin, filename);
    int fp = open(bin, 0);
    char script_file[BUF_SIZE * 4];
    read(fp, script_file, BUF_SIZE);
    char *line_read[];
    int num_lines = parse_split(line_read, script_file, '\n');
    for (int i = 0; i < num_lines; i++) {
        if (line_read[i][0] != '#' && line_read[i][1] != '!' && (int) strlen(line_read[i]) != 0) {
            shell_parse(line_read[i], (int) strlen(line_read[i]), envp);
        }
    }
    close(fp);
    return 0;
}

int shell_init(char* envp[]) {
    char input[BUF_SIZE], current_dir[BUF_SIZE];
    while (1) {
        memset(input, 0, BUF_SIZE);
        memset(current_dir, 0, BUF_SIZE);
        is_background = 0;
        getcwd(current_dir, BUF_SIZE);
        char *sbush = ": sbush> ";

        if (PS1_set) {
            write(STDOUT, PS1, strlen(PS1));
        }
        else {
            write(STDOUT, current_dir, strlen(current_dir));
            write(STDOUT, sbush, strlen(sbush));
        }

        read(STDIN, input, BUF_SIZE);

        if (*input != '\0') {
            int len_input = strlen(input);
            if (input[len_input - 1] == '\n') {
                input[len_input - 1] = 0;
                len_input--;
            }
            if (strcmp(input, "exit") == 0) {
                break;
            }
            else if (input[0] == '.' && input[1] == '/') {
                shell_execfile(&input[2], envp);
            }
            else if (input[0] == 's' && input[1] == 'b' && input[2] == 'u' && input[3] == 's' && input[4] == 'h' && input[5] == ' ') {
                shell_execfile(&input[6], envp);
            }
            else {
                shell_parse(input, len_input, envp);
            }
        }
        else {
            break;
        }
    }
    return 0;
}

int main(int argc, char *argv[], char *envp[]) {
    char *welcome_msg = "Welcome to SBUNIX!\n\n";
    write(STDOUT, welcome_msg, strlen(welcome_msg));
    if (argc == 2) {
        shell_execfile(argv[1], envp);
    }
    else if (argc > 0) {
        shell_init(envp);
    }
    while(1);
    return 0;
}
