#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>

#define STDIN 0
#define STDOUT 1
#define STDERR 2

#define BUF_SIZE 256
#define MAX_ARGS 16

struct job {
    char jobname[BUF_SIZE];
    char arguments[MAX_ARGS][BUF_SIZE];
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
    for (i = 0; (path[i] != '/' || path[i] != '\0'); i++) {
        bin[i] = path[i];
    }
    bin[i] = 0;
    strcat(bin, "/");
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
            waitpid(c_pid, NULL);
        }
    }
    return 0;
}

int set_env_variables(char *arguments[], int num_args, char *envp[]) {
    if (arguments[1][0] == 'P' && arguments[1][1] == 'S' && arguments[1][2] == '1') {
        char tokens[MAX_ARGS][BUF_SIZE];
        parse_split((char **) tokens, arguments[1], '=');
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
            char* error_msg = "cd: No such file or directory\n";
            write(STDIN, error_msg, strlen(error_msg));
        }
    }
    else {
        char* error_msg = "cd: Incorrect number of arguments\n";
        write(STDIN, error_msg, strlen(error_msg));
    }
    return 0;
}

int execute_job(struct job job_to_execute, char* envp[]) {
    char* jobname = job_to_execute.arguments[0];
    if (strcmp(jobname, "cd") == 0) {
        cd((char **) job_to_execute.arguments, job_to_execute.num_args, envp);
    }
    else if (strcmp(jobname, "export") == 0) {
        set_env_variables((char **) job_to_execute.arguments, job_to_execute.num_args, envp);
    }
    else {
        exec_binary((char **) job_to_execute.arguments, job_to_execute.num_args, envp);
    }
    return 0;
}

int shell_parse(char *input, int len_input, char *envp[]) {    
    if (strcmp(input, "$PATH") == 0) {
        char print_path[BUF_SIZE];
        strcpy(print_path, path);
        strcat(print_path, "\n");
        write(STDOUT, print_path, strlen(print_path));
        return 0;
    }
    else if (strcmp(input, "$PS1") == 0) {
        char print_PS1[BUF_SIZE];
        strcpy(print_PS1, PS1);
        strcat(print_PS1, "\n");
        write(STDOUT, print_PS1, strlen(print_PS1));
        return 0;
    }

    if (*input) {
        if (input[len_input - 1] == '&') {
            is_background = 1;
            input[len_input - 1] = 0;
            len_input--;
        }
    }

    input = remove_white_spaces(input);
    struct job shell_job;
    shell_job.num_args = parse_split((char **) shell_job.arguments, input, ' ');
    execute_job(shell_job, envp);
    return 0;
}

int shell_execfile(char *filename, char *envp[]) {
    int fp = open(filename, 0);
    char script_file[BUF_SIZE];
    read(fp, script_file, BUF_SIZE);
    char line_read[BUF_SIZE / 2][BUF_SIZE / 4];
    int num_lines = parse_split((char **) line_read, script_file, '\n');
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
        char *dir = getcwd(current_dir, BUF_SIZE);
        strcat(dir, ": sbush> ");

        if (PS1_set) {
            write(STDOUT, PS1, strlen(PS1));
        }
        else {
            write(STDOUT, dir, strlen(dir));
        }

        read(STDIN, input, BUF_SIZE);

        if (*input) {
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
