//
// Created by Saraj Munjal on 11/15/17.
//

#ifndef PROJECT_KTHREAD_H

#include <sys/defs.h>

#define PROJECT_KTHREAD_H
#define K_STACK_SIZE 4096
typedef struct kern_thread {
    uint64_t *rsp_val;
    uint64_t k_stack[K_STACK_SIZE];
}__attribute__((__packed__)) kthread_t;

void test_context_switch();

#endif //PROJECT_KTHREAD_H
