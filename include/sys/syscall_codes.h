#ifndef SYSCALL_CODES_H
#define SYSCALL_CODES_H

typedef enum syscall_code {
    SYSCALL_READ = 0x1,
    SYSCALL_WRITE = 0x2,
    SYSCALL_OPEN = 0x3,
    SYSCALL_CLOSE = 0x4,
    SYSCALL_UNLINK = 0x5,
    SYSCALL_CHDIR = 0x6,
    SYSCALL_GETCWD = 0x7,
    SYSCALL_FORK = 0x8,
    SYSCALL_EXECVPE = 0x9,
    SYSCALL_WAIT = 0xA,
    SYSCALL_YIELD = 0xB
} syscall_code_t;
#endif
