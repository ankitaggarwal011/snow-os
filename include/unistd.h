#ifndef _UNISTD_H
#define _UNISTD_H

#include <sys/defs.h>
#include <sys/syscall_codes.h>

uint64_t syscall1(syscall_code_t code);

uint64_t syscall2(syscall_code_t code, uint64_t arg2);

uint64_t syscall3(syscall_code_t code, uint64_t arg2, uint64_t arg3);

uint64_t syscall4(syscall_code_t code, uint64_t arg2, uint64_t arg3, uint64_t arg4);

uint64_t syscall5(syscall_code_t code, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5);

uint64_t syscall6(syscall_code_t code, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg6);

#define FILE_OPEN_FLAG_O_DIRECTORY //enforce that file is not a dir

int open(const char *pathname, int flags);

int close(int fd);

ssize_t read(int fd, void *buf, size_t count);

ssize_t write(int fd, const void *buf, size_t count);

int chdir(const char *path);

char *getcwd(char *buf, size_t size);

pid_t fork();

int yield();
int execvpe(const char *file, char *const argv[], char *const envp[]);
pid_t wait(int *status);
int waitpid(int pid, int *status);

unsigned int sleep(unsigned int seconds);

pid_t getpid(void);

pid_t getppid(void);

void get_process_state(char *buf);

void exit(int status);

int kill(int pid);
// OPTIONAL: implement for ``on-disk r/w file system (+10 pts)''
//off_t lseek(int fd, off_t offset, int whence);
//int mkdir(const char *pathname, mode_t mode);
//int unlink(const char *pathname);
// OPTIONAL: implement for ``signals and pipes (+10 pts)''
//int pipe(int pipefd[2]);

#endif
