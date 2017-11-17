#include <sys/kthread.h>
#include <sys/kprintf.h>
#include <sys/paging.h>

extern void switch_to(kthread_t **me, kthread_t *next);

extern void set_rsp(uint64_t val);

void test_func_1();

void test_func_2();

void schedule();


kthread_t *t1, *t2;
kthread_t *cur = NULL;
//kthread_t *last = NULL;

void test_func_1() {
    int i = 0;
    while (1) {
        i++;
        if (i % 100 == 0) {
            kprintf("Func 1: %d ", i);
        }
        if (i % 200 == 0) {
            schedule();
        }
    }
}

void test_func_2() {
    int i = 0;
    while (1) {
        i++;
        if (i % 100 == 0) {
            kprintf("Func 2: %d ", i);
        }
        if (i % 200 == 0) {
            schedule();
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

    t2 = (kthread_t *) kmalloc(sizeof(kthread_t));
    memset(t2->k_stack, 0, K_STACK_SIZE);
    uint64_t def_val = 0x0;
    t2->k_stack[K_STACK_SIZE - 1] = (uint64_t) test_func_2;
    for (int i = 1; i <= 16; i++) {
        t2->k_stack[K_STACK_SIZE - i - 1] = def_val;
    }
    t2->rsp_val = &(t2->k_stack[K_STACK_SIZE - 17]);
}


void test_context_switch() {
//    last = &t1;
    kprintf("Getting here\n");
    init_kthreads();
    set_rsp((uint64_t) t1->rsp_val);
}