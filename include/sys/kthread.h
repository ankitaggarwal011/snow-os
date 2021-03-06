#ifndef _KTHREAD_H
#define _KTHREAD_H

#include <sys/defs.h>
#include <sys/vfs.h>

#define PROJECT_KTHREAD_H
#define K_STACK_SIZE 4096
#define NUM_FDS 16
#define BUF_SIZE 128

typedef enum process_state_e {
    RUNNING = 0x0,
    ZOMBIE = 0x1,
    QUEUED = 0x2
} process_state;
// WARNING: don't add anything at the start of kern_thread struct. Register logic assumes that the offset of rsp_val is 0.
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
    char *process_name;
    struct kern_thread *next;
    file_object_t *fds[NUM_FDS];
    char cwd[BUF_SIZE];
    process_state state;
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
    struct vma_struct *vma_stack;
    struct vma_struct *vma_heap;
}__attribute__((__packed__));

// void test_context_switch();

extern void set_rsp(uint64_t val);

extern void switch_to(kthread_t **me, kthread_t *next);

#endif
