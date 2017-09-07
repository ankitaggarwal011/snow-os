/* Author: Ankit Aggarwal */

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<dirent.h>
#include<sys/wait.h>
#include<fcntl.h>

#define BUF_SIZE 1024
#define PROT_READ	0x1
#define PROT_WRITE	0x2
#define MAP_PRIVATE	0x02
#define MAP_ANONYMOUS	0x20

// #define O_RDONLY 0x0000

#define SYS_READ 0
#define SYS_WRITE 1
#define SYS_OPEN 2
#define SYS_CLOSE 3
#define SYS_FORK 57
#define SYS_WAIT4 61
#define SYS_GETCWD 79
#define SYS_CHDIR 80
#define SYS_EXECVE 59
#define SYS_EXIT 60
#define SYS_PIPE 22
#define SYS_DUP2 33
#define SYS_MMAP 9
#define SYS_MUNMAP 11

long cus_write(long f, void* buffer, unsigned long size) {
    long return_val;
    __asm__ volatile
    (
        "syscall"
        : "=a" (return_val)
        : "0"(SYS_WRITE), "D"(f), "S"(buffer), "d"(size)
        : "cc", "r11", "rcx", "memory"
    );
    return return_val;
}

long cus_read(long f, void* buffer, unsigned long size) {
    long return_val;
    __asm__ volatile
    (
        "syscall"
        : "=a" (return_val)
        : "0"(SYS_READ), "D"(f), "S"(buffer), "d"(size)
        : "cc", "r11", "rcx", "memory"
    );
    return return_val;
}

long cus_open(char* pathname, void* buffer) {
    long return_val;
    __asm__ volatile
    (
        "syscall"
        : "=a" (return_val)
        : "0"(SYS_OPEN), "D"(pathname), "S"(buffer)
        : "cc", "r11", "rcx", "memory"
    );
    return return_val;
}

long cus_close(long f) {
    long return_val;
    __asm__ volatile
    (
        "syscall"
        : "=a" (return_val)
        : "0"(SYS_CLOSE), "D"(f)
        : "cc", "r11", "rcx", "memory"
    );
    return return_val;
}

char* cus_getcwd(char* buffer, unsigned long size) {
    long return_val;
    __asm__ volatile
    (
        "syscall"
        : "=a" (return_val)
        : "0"(SYS_GETCWD), "D"(buffer), "S"(size)
        : "cc", "r11", "rcx", "memory"
    );
    if (return_val != -1) return buffer;
    else return NULL;
}

long cus_chdir(char* buffer) {
    long return_val;
    __asm__ volatile
    (
        "syscall"
        : "=a" (return_val)
        : "0"(SYS_CHDIR), "D"(buffer)
        : "cc", "r11", "rcx", "memory"
    );
    return return_val;
}

long cus_fork(void) {
    long return_val;
    __asm__ volatile
    (
        "syscall"
        : "=a" (return_val)
        : "0"(SYS_FORK)
        : "cc", "r11", "rcx", "memory"
    );
    return return_val;
}

long cus_waitpid(long pid, void* status, long options) {
    long return_val;
    __asm__ volatile
    (
        "syscall"
        : "=a" (return_val)
        : "0"(SYS_WAIT4), "D"(pid), "S"(status), "d"(options)
        : "cc", "r11", "rcx", "memory"
    );
    return return_val;
}

long cus_execve(char* name, char* arguments[], char* envp[]) {
    long return_val;
    __asm__ volatile
    (
        "syscall"
        : "=a" (return_val)
        : "0"(SYS_EXECVE), "D"(name), "S"(arguments), "d"(envp)
        : "cc", "r11", "rcx", "memory"
    );
    return return_val;
}

long cus_exit(long status) {
    long return_val;
    __asm__ volatile
    (
        "syscall"
        : "=a" (return_val)
        : "0"(SYS_EXIT), "D"(status)
        : "cc", "r11", "rcx", "memory"
    );
    return return_val;
}

long cus_pipe(int filedes[2]) {
    long return_val;
    __asm__ volatile
    (
        "syscall"
        : "=a" (return_val)
        : "0"(SYS_PIPE), "D"(filedes)
        : "cc", "r11", "rcx", "memory"
    );
    return return_val;
}

long cus_dup2(long oldfd, long newfd) {
    long return_val;
    __asm__ volatile
    (
        "syscall"
        : "=a" (return_val)
        : "0"(SYS_DUP2), "D"(oldfd), "S"(newfd)
        : "cc", "r11", "rcx", "memory"
    );
    return return_val;
}

long cus_mmap(void* start, long length, long prot, long flags, long fd, long offset) {
    long return_val;
    register long r8 __asm__("r8") = fd; // Add data in this sequence only
    register long r9 __asm__("r9") = offset;
    __asm__ volatile
    (
        "syscall"
        : "=a" (return_val)
        : "0"(SYS_MMAP), "D"(start), "S"(length), "d"(prot), "c"(flags), "r"(r8), "r"(r9)
        : "cc", "r11", "memory"
    );
    return return_val;
}

void* cus_malloc(size_t size) {
    void* return_val = (void *) cus_mmap(0, size + sizeof(size_t), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    *(size_t*)return_val = size;
    return return_val + sizeof(size_t);
}

struct command_table {
    struct job** jobs;
    int num_jobs;
};

struct job {
    char* jobname;
    int num_args;
    char** arguments;
};

char PS1[BUF_SIZE] = "cwd: sbush> ";
char root_path[BUF_SIZE];
int PS1_set = 0;
int is_background;
int path_num;

int put_env(char* new_path, char* envp[]) {
    int i = 5;
    while(new_path[i]) {
        root_path[i - 5] = new_path[i];
        i++;
    }
    while(root_path[i - 5]) root_path[i - 5] = 0;
    envp[path_num] = new_path;
    return 0;
}

char* remove_white_spaces(char* src) {
    while (*src == ' ') {
        ++src;
    }
    int i = strlen(src) - 1;
    while (src[i] == ' ') {
        src[i] = 0;
        i--;
    }
    return src;
}

int parse_split(char** commands, char* input, const char delimiter) {
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

int exec_binary(char** arguments, int num_args, char* envp[]) {
    char* temp = (char*) malloc(sizeof(char) * BUF_SIZE);
    int i = 0;
    while(root_path[i] && root_path[i] != ':') {
        temp[i] = root_path[i];
        i++;
    }
    temp[i] = 0;
    strcat(temp, "/");
    strcat(temp, arguments[0]);
    pid_t c_pid = fork();
    if (c_pid == 0) {
        execve(temp, arguments, envp);
        char* temp_var = "Command not found\n";
        if (write(1, temp_var, sizeof(char) * strlen(temp_var)) == -1) { /* Write error */ }
        exit(0);
    }
    else {
        if (is_background == 0) {
            while (waitpid(c_pid, NULL, 0) > 0);
        }
    }
    return 0;
}

int set_env_variables(char** arguments, int num_args, char* envp[]) {
    if (arguments[1][0] == 'P' && arguments[1][1] == 'S' && arguments[1][2] == '1') {
        char** tokens = (char**) malloc(sizeof(char*) * BUF_SIZE);
        parse_split(tokens, arguments[1], '=');
        strcpy(PS1, tokens[1]);
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
            char* temp_var = "sbush: export: Error setting environment variable\n";
            if (write(1, temp_var, sizeof(char) * strlen(temp_var)) == -1) { /* Write error */ }
        }
    }
    return 0;
}

int cd(char** arguments, int num_args, char* envp[]) {
    int status;
    if (num_args == 2) {
        if (arguments[1][0] == '/') {
            status = chdir(arguments[1]);
        }
        else {
            char* current_dir;
            current_dir = (char*) malloc(sizeof(char) * BUF_SIZE);
            current_dir = getcwd(current_dir, BUF_SIZE);
            strcat(current_dir, "/");
            strcat(current_dir, arguments[1]);
            status = chdir(current_dir);
        }
        if (status != 0) {
            char* temp_var = "sbush: cd: No such file or directory\n";
            if (write(1, temp_var, sizeof(char) * strlen(temp_var)) == -1) { /* Write error */ }
        }
    }
    else {
        char* temp_var = "sbush: cd: Incorrect number of arguments\n";
        if (write(1, temp_var, sizeof(char) * strlen(temp_var)) == -1) { /* Write error */ }
    }
    return 0;
}

int execute_job(struct job* job_to_execute, char* envp[]) {
    char* jobname = job_to_execute->arguments[0];
    if (strcmp(jobname, "cd") == 0) {
        cd(job_to_execute->arguments, job_to_execute->num_args, envp);
    }
    else if (strcmp(jobname, "export") == 0) {
        set_env_variables(job_to_execute->arguments, job_to_execute->num_args, envp);
    }
    else {
        exec_binary(job_to_execute->arguments, job_to_execute->num_args, envp);
    }
    return 0;
}

int execute_piped_jobs(struct job** jobs_to_execute, int num_jobs, char* envp[]) {
    for (int i = 0; i < num_jobs; i++) {
        int pipe_fd[2];
        if (i < num_jobs - 1) {
            if (pipe(pipe_fd) != 0) {
                char* temp_var = "Unable to create pipes\n";
                if (write(1, temp_var, sizeof(char) * strlen(temp_var)) == -1) { /* Write error */ }
            }
        }
        pid_t c_pid = fork();
        if (c_pid == 0) {
            if (i < num_jobs - 1) {
                if (dup2(pipe_fd[1], STDOUT_FILENO) == -1) {
                    char* temp_var = "Failed to duplicate file descriptor\n";
                    if (write(1, temp_var, sizeof(char) * strlen(temp_var)) == -1) { /* Write error */ }
                }
            }
            else {
                close(pipe_fd[1]);
            }
            close(pipe_fd[0]);
            char* temp = (char*) malloc(sizeof(char) * BUF_SIZE);
            int j = 0;
            while(root_path[j] && root_path[j] != ':') {
                temp[j] = root_path[j];
                j++;
            }
            temp[j] = 0;
            strcat(temp, "/");
            strcat(temp, jobs_to_execute[i]->arguments[0]);
            execve(temp, jobs_to_execute[i]->arguments, envp);
            char* temp_var = "Command not found\n";
            if (write(1, temp_var, sizeof(char) * strlen(temp_var)) == -1) { /* Write error */ }
            exit(0);
        }
        else {
            if (i < num_jobs - 1) {
                dup2(pipe_fd[0], STDIN_FILENO);
            }
            else {
                close(pipe_fd[0]);
            }
            close(pipe_fd[1]);
            if (is_background == 0) {
                while (waitpid(c_pid, NULL, 0) > 0);
            }
        }
    }
    return 0;
}

int execute_jobs(struct command_table* ctable, char* envp[]) {
    if (ctable->num_jobs == 1) {
        execute_job(ctable->jobs[0], envp);
    }
    else {
        execute_piped_jobs(ctable->jobs, ctable->num_jobs, envp);
    }
    return 0;
}

int shell_parse(char* input, int len_input, char* envp[]) {
    int i = 0;
    while(envp[i]) {
        if (envp[i][0] == 'P' && envp[i][1] == 'A' && envp[i][2] == 'T' && envp[i][3] == 'H') {
            int j = 5;
            while (envp[i][j]) {
                root_path[j - 5] = envp[i][j];
                j++;
            }
            path_num = i;
            break;
        }
        i++;
    }
    if (strcmp(input, "$PATH") == 0) {
        char* temp_var = root_path;
        strcat(temp_var, "\n");
        if (write(1, temp_var, sizeof(char) * strlen(temp_var)) == -1) { /* Write error */ }
        return 0;
    }
    else if (strcmp(input, "$PS1") == 0) {
        char* temp_var = PS1;
        strcat(temp_var, "\n");
        if (write(1, temp_var, sizeof(char) * strlen(temp_var)) == -1) { /* Write error */ }
        return 0;
    }
    struct command_table* ctable = (struct command_table*) malloc(sizeof(struct command_table) * 1);
    ctable->jobs = (struct job**) malloc(sizeof(struct job*) * BUF_SIZE);

    if (input && *input) {
        if (input[len_input - 1] == '&') {
            is_background = 1;
            input[len_input - 1] = 0;
            len_input--;
        }
    }
    input = remove_white_spaces(input);
    len_input = strlen(input);
    if (input[len_input - 1] == '|') {
        char* temp_var = "Invalid command syntax: |\n";
        if (write(1, temp_var, sizeof(char) * strlen(temp_var)) == -1) { /* Write error */ }
        return 0;
    }
    char** commands = (char**) malloc(sizeof(char*) * BUF_SIZE);
    int num_commands = parse_split(commands, input, '|');
    ctable->num_jobs = num_commands;
    for (int i = 0; i < num_commands; i++) {
        char** tokens = (char**) malloc(sizeof(char*) * BUF_SIZE);
        int num_tokens = parse_split(tokens, commands[i], ' ');
        struct job* single_job = (struct job*) malloc(sizeof(struct job) * 1);
        single_job->arguments = tokens;
        single_job->num_args = num_tokens;
        ctable->jobs[i] = single_job;
    }
    execute_jobs(ctable, envp);
    return 0;
}

int shell_init(char *envp[]) {
    while (1) {
        char* input = (char*) malloc(sizeof(char) * BUF_SIZE);
        char* current_dir = (char*) malloc(sizeof(char) * BUF_SIZE);
        is_background = 0;
        current_dir = getcwd(current_dir, BUF_SIZE);
        strcat(current_dir, ": sbush> ");
        if (PS1_set) {
            char* temp_var = PS1;
            if (write(1, temp_var, sizeof(char) * strlen(temp_var)) == -1) { /* Write error */ }
        }
        else {
            char* temp_var = current_dir;
            if (write(1, temp_var, sizeof(char) * strlen(temp_var)) == -1) { /* Write error */ }
        }
        if (read(0, input, BUF_SIZE) == -1) { /* Read error*/ }
        if (input && *input) {
            int len_input = strlen(input);
            if (input[len_input - 1] == '\n') {
                input[len_input - 1] = 0;
                len_input--;
            }
            if (strcmp(input, "exit") == 0) {
                break;
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

int shell_execfile(char* filename, char *envp[]) {
    int fp = open(filename, O_RDONLY);
    char* script_file = (char*) malloc(sizeof(char) * BUF_SIZE);
    if (read(fp, script_file, BUF_SIZE) != -1) {
        char** line_read = (char**) malloc(sizeof(char*) * BUF_SIZE);
        int num_lines = parse_split(line_read, script_file, '\n');
        for (int i = 0; i < num_lines; i++) {
            if (line_read[i][0] != '#' && line_read[i][1] != '!' && (int) strlen(line_read[i]) != 0) {
                shell_parse(line_read[i], (int) strlen(line_read[i]), envp);
            }
        }
    }
    close(fp);
    return 0;
}

int main(int argc, char *argv[], char *envp[]) {
    if (argc == 2) {
        shell_execfile(argv[1], envp); // Todo: file validation
    }
    else if (argc > 2) {
        char* temp_var = "Incorrect number of command line arguments. Please try again.\n";
        if (write(1, temp_var, sizeof(char) * strlen(temp_var)) == -1) { /* Write error */ }
    }
    else {
        shell_init(envp);
    }
    return 0;
}
