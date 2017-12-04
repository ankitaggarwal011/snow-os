#include <sys/kthread.h>
#include <sys/kprintf.h>
#include <sys/paging.h>
#include <sys/syscall_codes.h>
#include <sys/string.h>
#include <unistd.h>
#include <sys/gdt.h>

extern void set_rsp_arg1(uint64_t rsp_val, uint64_t arg1);
extern void _jump_usermode(void *starting_func_addr);

extern uint64_t test_syscall();

void test_func_1();

void test_func_2();

void schedule();

void user_test_func();

kthread_t *t1, *t2;
kthread_t *cur = NULL;
//kthread_t *last = NULL;

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

void load_user_func(void* func) {
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

    t2 = (kthread_t *) kmalloc(sizeof(kthread_t));
    memset(t2->k_stack, 0, K_STACK_SIZE);
    uint64_t def_val = 0x0;
    t2->k_stack[K_STACK_SIZE - 1] = (uint64_t) test_func_2;
    for (int i = 1; i <= 16; i++) {
        t2->k_stack[K_STACK_SIZE - i - 1] = def_val;
    }
    t2->rsp_val = &(t2->k_stack[K_STACK_SIZE - 17]);
}

void user_test_func() {
    // char *str = "hellio";
    // kprintf("In user space\n");
    // write(1, (void *) str, strlen(str));
}

void test_context_switch() {
//    last = &t1;
    kprintf("Getting here\n");
    init_kthreads();
    set_rsp((uint64_t) t1->rsp_val);
}


kthread_t *get_cur_kthread() {
    return cur;
}

void go_to_ring3(kthread_t *user_binary) {
    set_tss_rsp(&(user_binary->k_stack[K_STACK_SIZE - 1]));
    set_new_cr3(user_binary->cr3);

    __asm__ __volatile__ (
        "movq %0, %%rax;"
        "pushq $0x23;"
        "pushq %%rax;"
        "pushfq;"
        "popq %%rax;"
        "pushq %%rax;"
        "pushq $0x2B;"
        "pushq %1;"
        "iretq;"
        ::"r"(user_binary->rsp_user),"r"(user_binary->rip)
    );
}