#ifndef _KTHREAD_H
#define _KTHREAD_H

#include <sys/defs.h>

#define K_STACK_SIZE 4096
typedef struct kern_thread {
    uint64_t *rsp_val;
    uint64_t rsp_user;
    uint64_t k_stack[K_STACK_SIZE];
    int pid;
    uint64_t cr3;
    struct mm_struct *process_mm;
    uint64_t rip;
    int ppid;
    int num_child;
    struct kern_thread *next;
}__attribute__((__packed__)) kthread_t;

struct vma_struct {
    uint64_t start;
    uint64_t end;
    uint64_t type;
    uint64_t flags;
    struct vma_struct *next;
}__attribute__((__packed__));

struct mm_struct {
    struct vma_struct *vma_map;
    struct vma_struct *vma_map_iter;
}__attribute__((__packed__));

void test_context_switch();

void go_to_ring3();

kthread_t *get_cur_kthread();

extern void set_rsp(uint64_t val);

extern void switch_to(kthread_t **me, kthread_t *next);

#endif
