#ifndef PROJECT_KTHREAD_H

#include <sys/defs.h>
#include <sys/process.h>

#define PROJECT_KTHREAD_H
#define K_STACK_SIZE 4096
typedef struct kern_thread {
    uint64_t *rsp_val;
    uint64_t k_stack[K_STACK_SIZE];
    int pid;
    uint64_t cr3;
    struct mm_struct *process_mm;
    uint64_t rip;
}__attribute__((__packed__)) kthread_t;

void test_context_switch();

#endif //PROJECT_KTHREAD_H
