#include <sys/defs.h>
#include <sys/syscall_codes.h>

uint64_t syscall6(syscall_code_t code, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5, uint64_t arg6) {
    uint64_t return_val;
    __asm__ volatile (
    "movq %1, %%rax;"
            "movq %2, %%rbx;"
            "movq %3, %%rcx;"
            "movq %4, %%rdx;"
            "movq %5, %%rsi;"
            "movq %6, %%rdi;"
            "INT $0x80;"
            "movq %%rax,%0"
    : "=m" (return_val)
    : "m"(code), "m" (arg2), "m" (arg3), "m" (arg4), "m" (arg5), "m" (arg6)
    : "%rax", "%rbx", "%rcx", "%rdx", "%rsi", "%rdi"
    );
    return return_val;
}


uint64_t syscall5(syscall_code_t code, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5) {
    uint64_t return_val;
    __asm__ volatile (
    "movq %1, %%rax;"
            "movq %2, %%rbx;"
            "movq %3, %%rcx;"
            "movq %4, %%rdx;"
            "movq %5, %%rsi;"
            "INT $0x80;"
            "movq %%rax,%0"
    : "=m" (return_val)
    : "m"(code), "m" (arg2), "m" (arg3), "m" (arg4), "m" (arg5)
    : "%rax", "%rbx", "%rcx", "%rdx", "%rsi"
    );
    return return_val;
}

uint64_t syscall4(syscall_code_t code, uint64_t arg2, uint64_t arg3, uint64_t arg4) {
    uint64_t return_val;
    __asm__ volatile (
    "movq %1, %%rax;"
            "movq %2, %%rbx;"
            "movq %3, %%rcx;"
            "movq %4, %%rdx;"
            "INT $0x80;"
            "movq %%rax,%0"
    : "=m" (return_val)
    : "m"(code), "m" (arg2), "m" (arg3), "m" (arg4)
    : "%rax", "%rbx", "%rcx", "%rdx"
    );
    return return_val;
}

uint64_t syscall3(syscall_code_t code, uint64_t arg2, uint64_t arg3) {
    uint64_t return_val;
    __asm__ volatile (
    "movq %1, %%rax;"
            "movq %2, %%rbx;"
            "movq %3, %%rcx;"
            "INT $0x80;"
            "movq %%rax,%0"
    : "=m" (return_val)
    : "m"(code), "m" (arg2), "m" (arg3)
    : "%rax", "%rbx", "%rcx"
    );
    return return_val;
}

uint64_t syscall2(syscall_code_t code, uint64_t arg2) {
    uint64_t return_val;
    __asm__ volatile (
    "movq %1, %%rax;"
            "movq %2, %%rbx;"
            "INT $0x80;"
            "movq %%rax,%0"
    : "=m" (return_val)
    : "m"(code), "m" (arg2)
    : "%rax", "%rbx"
    );
    return return_val;
}

uint64_t syscall1(syscall_code_t code) {
    uint64_t return_val;
    __asm__ volatile (
    "movq %1, %%rax;"
            "INT $0x80;"
            "movq %%rax,%0"
    : "=m" (return_val)
    : "m"(code)
    : "%rax"
    );
    return return_val;
}