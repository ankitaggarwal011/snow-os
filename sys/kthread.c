#include <sys/kthread.h>
#include <sys/kprintf.h>
#include <sys/paging.h>
#include <sys/syscall_codes.h>
#include <sys/string.h>
#include <unistd.h>
#include <sys/vfs.h>
#include <sys/gdt.h>

extern void set_rsp_arg1(uint64_t rsp_val, uint64_t arg1);

extern void _jump_usermode(void *starting_func_addr);

/*
extern uint64_t test_syscall();

void test_func_1();

void test_func_2();

void user_test_func();

void schedule();

kthread_t *t1, *t2;
kthread_t *cur = NULL;

void test_func_1() {
    unsigned int i = 0;
    while (1) {
        i++;
        if (i < 100000) {
            if (i % 10000 == 0) {
                kprintf("Func 1: %d ", i);
            }
            if (i % 20000 == 0) {
                schedule();
                _jump_usermode(user_test_func);
            }
        }

    }
}

void load_user_func(void *func) {
    _jump_usermode(func);
}

void test_func_2() {
    unsigned int i = 0;
    while (1) {
        i++;
        if (i < 100000) {
            if (i % 10000 == 0) {
                kprintf("Func 2: %d ", i);
            }
            if (i % 20000 == 0) {
                schedule();
            }
        }
    }
}


void schedule() {
    if (cur == NULL) {
        cur = t1;
        switch_to(&cur, t2);
    } else if (cur == t1) {
        switch_to(&cur, t2);
    } else if (cur == t2) {
        switch_to(&cur, t1);
    }
    //last updated automatically
    // cur updated automatically
}

void init_kthreads() {
    kprintf("Size of kthread is %d\n", sizeof(kthread_t));
    t1 = (kthread_t *) kmalloc(sizeof(kthread_t));
    memset(t1->k_stack, 0, K_STACK_SIZE);
    t1->k_stack[K_STACK_SIZE - 1] = (uint64_t) test_func_1;
    t1->rsp_val = &(t1->k_stack[K_STACK_SIZE - 1]);
    file_object_t *stdin_fo = get_stdin_fo();
    stdin_fo->ref_count++;
    t1->fds[0] = stdin_fo;

    file_object_t *stdout_fo = get_stdout_fo();

    stdout_fo->ref_count++;
    t1->fds[1] = stdout_fo;
    t2 = (kthread_t *) kmalloc(sizeof(kthread_t));
    memset(t2->k_stack, 0, K_STACK_SIZE);
    uint64_t def_val = 0x0;
    t2->k_stack[K_STACK_SIZE - 1] = (uint64_t) test_func_2;
    for (int i = 1; i <= 16; i++) {
        t2->k_stack[K_STACK_SIZE - i - 1] = def_val;
    }
    t2->rsp_val = &(t2->k_stack[K_STACK_SIZE - 17]);
    stdin_fo->ref_count++;
    t2->fds[0] = stdin_fo;
    stdout_fo->ref_count++;
    t2->fds[1] = stdout_fo;
}

void test_context_switch() {
    kprintf("Getting here\n");
    init_kthreads();
    set_rsp((uint64_t) t1->rsp_val);
}
*/

kthread_t *get_cur_kthread() {
    return cur;
}
