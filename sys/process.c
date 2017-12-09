#include <sys/defs.h>
#include <sys/kprintf.h>
#include <sys/physical_memory.h>
#include <sys/string.h>
#include <sys/tarfs.h>
#include <sys/elf64.h>
#include <sys/process.h>
#include <sys/kthread.h>
#include <sys/paging.h>
#include <sys/gdt.h>

#define BUF_SIZE 128
#define MAX_ARGS 8

kthread_t *current_process;

kthread_t *get_current_process() {
    return current_process;
}

extern void setup_forked_kthread_stack(uint64_t *addr);

extern uint64_t get_rsp_val();

extern void put_in_rax();

int getPID() {
    for (int i = 1; i < MAX_P; i++) {
        if (processes[i] == 0) {
            processes[i] = 1;
            return i;
        }
    }
    return -1;
}

int get_process_pid() {
    return current_process->pid;
}

int get_process_ppid() {
    return current_process->ppid;
}

void init_processes() {
    for (int i = 0; i < MAX_P; i++) {
        processes[i] = 0;
    }
}

void init_scheduler() {
    while (1) {
        // kprintf(" _idle_t_ ");
        scheduler();
    }
}

void scheduler() {
    switch_process();
}

void switch_process() {
    if (current_process == current_process->next) {
        // only one process in system. No problem.
        return;
    }
    // find the next process that is not a zombie and is queued.
    kthread_t *cand = current_process->next;
    while (cand != current_process && cand->state != QUEUED) {
        cand = cand->next;
    }
    if (cand == current_process) {
        // went around all processes. Nobody was in QUEUED state!
        kprintf("No queued process found for running!!!\n");
        return;
    }

    if (current_process->state == RUNNING) {
        current_process->state = QUEUED;
    }
    else if (current_process->state != ZOMBIE) {
        kprintf("Some problem with process states: %d!! \n", current_process->state);
        return;
    }
    cand->state = RUNNING;
    set_new_cr3(cand->cr3);
    set_tss_rsp(&cand->k_stack[K_STACK_SIZE - 1]);
    switch_to(&current_process, cand);
}

kthread_t *init_idle_process() {
    kthread_t *idle = (kthread_t *) kmalloc(sizeof(kthread_t));
    memset(idle->k_stack, 0, K_STACK_SIZE);
    idle->k_stack[K_STACK_SIZE - 1] = (uint64_t) & init_scheduler;
    idle->rsp_val = &(idle->k_stack[K_STACK_SIZE - 1]);
    idle->rsp_user = (uint64_t) & (idle->k_stack[K_STACK_SIZE - 1]);
    idle->pid = getPID();
    idle->ppid = 0;
    idle->process_mm = NULL;
    idle->cr3 = (uint64_t) get_cr3();
    idle->rip = (uint64_t) & init_scheduler;
    idle->next = idle;
    idle->num_child = 0;
    current_process = idle;
    file_object_t *stdin_fo = get_stdin_fo();
    stdin_fo->ref_count++;
    idle->fds[0] = stdin_fo;
    idle->process_name = "Idle";
    file_object_t *stdout_fo = get_stdout_fo();
    stdout_fo->ref_count++;
    idle->fds[1] = stdout_fo;
    idle->fds[2] = stdout_fo;
    idle->state = RUNNING;
    return idle;
}

kthread_t *create_process(char *filename) {
    kthread_t *new_process = (kthread_t *) kmalloc(sizeof(kthread_t));
    memset(new_process->k_stack, 0, K_STACK_SIZE);
    new_process->k_stack[K_STACK_SIZE - 1] = (uint64_t) & go_to_ring3;
    new_process->rsp_val = &(new_process->k_stack[K_STACK_SIZE - 17]);
    new_process->pid = getPID();
    new_process->next = NULL;
    new_process->num_child = 0;

    int i = 0;
    char *tmp = filename;
    while (*tmp != 0) tmp++;
    while (*tmp != '/') tmp--;

    char p_name[BUF_SIZE];
    int len = strlen(filename);
    for (int i = 0; i < len; i++) {
        p_name[i] = filename[i];
    }
    p_name[len] = '\0';
    //p_name[0] = 0;
    //strcat((char *) p_name, filename);

    new_process->process_name = (char *) &p_name;
    for (i = 0; (filename + i) < tmp; i++) {
        new_process->cwd[i] = *(filename + i);
    }
    new_process->cwd[i] = 0;
    new_process->cr3 = setup_user_page_tables();
    uint64_t current_cr3 = get_cr3();
    set_new_cr3(new_process->cr3);
    struct mm_struct *process_mm = (struct mm_struct *) kmalloc(sizeof(struct mm_struct));
    new_process->process_mm = process_mm;
    file_object_t *stdin_fo = get_stdin_fo();
    stdin_fo->ref_count++;
    new_process->fds[0] = stdin_fo;

    file_object_t *stdout_fo = get_stdout_fo();
    stdout_fo->ref_count++;
    new_process->fds[1] = stdout_fo;
    new_process->fds[2] = stdout_fo;
    load_file(new_process, filename);

    set_new_cr3(current_cr3);
    new_process->state = QUEUED;
    current_process->next = new_process;
    new_process->next = current_process;

    return new_process;
}

uint64_t user_malloc(uint64_t bytes) {
    uint64_t start_alloc = current_process->process_mm->vma_map_iter->end;
    uint64_t end_alloc = start_alloc;
    if (bytes % PAGE_SIZE == 0) {
        end_alloc += bytes;
    } else {
        end_alloc += ((bytes / PAGE_SIZE) + 1) * PAGE_SIZE;
    }
    struct vma_struct *vma_malloc = kmalloc(sizeof(struct vma_struct));
    vma_malloc->start = start_alloc;
    vma_malloc->end = end_alloc;
    vma_malloc->next = NULL;
    current_process->process_mm->vma_map_iter->next = vma_malloc;
    current_process->process_mm->vma_map_iter = vma_malloc;

    while (start_alloc < end_alloc) {
        update_page_tables(start_alloc, get_free_page(), PAGING_USER_R_W_FLAGS);
        start_alloc += PAGE_SIZE;
    }

    return vma_malloc->start;
}

void user_free(uint64_t addr) {
    struct vma_struct *iter = current_process->process_mm->vma_heap;
    while (iter != NULL) {
        if (iter->start == addr) {
            uint64_t tmp = addr;
            while (tmp < iter->end) {
                kfree((void *) tmp);
                tmp += PAGE_SIZE;
            }
            break;
        }
        iter = iter->next;
    }
}

int get_cwd(char *buf, size_t size) {
    for (int i = 0; i < size; i++) {
        *(buf + i) = *(current_process->cwd + i);
    }
    return 0;
}

int ch_dir(char *path) { // TODO: check if a directory exists
    int i = 0;
    for (i = 0; *(path + i) != 0; i++) {
        current_process->cwd[i] = *(path + i);
    }
    current_process->cwd[i] = 0;
    return 0;
}

void go_to_ring3() {
    set_tss_rsp(&current_process->k_stack[K_STACK_SIZE - 1]);
    set_new_cr3(current_process->cr3);
    // current_process->state will already be running;
    __asm__ __volatile__ (
    "movq %1, %%rax;"
            "pushq $0x23;"
            "pushq %%rax;"
            "pushfq;"
            "pushq $0x2B;"
            "pushq %0;"
            "iretq;"
    ::"r"(current_process->rip), "r"(current_process->rsp_user)
    );
}

uint64_t copy_process(kthread_t *parent_task) {
    kthread_t *child = (kthread_t *) kmalloc(sizeof(kthread_t));
    memset(child->k_stack, 0, K_STACK_SIZE);
    child->rsp_val = &(child->k_stack[K_STACK_SIZE - 1]);
    child->rsp_user = parent_task->rsp_user;
    child->pid = getPID();
    child->ppid = parent_task->pid;
    child->process_mm = NULL;
    child->next = NULL;
    child->num_child = 0;
    file_object_t *stdin_fo = get_stdin_fo();
    stdin_fo->ref_count++;
    child->fds[0] = stdin_fo;

    char p_name[BUF_SIZE];
    p_name[0] = 0;
    strcat((char *) p_name, parent_task->process_name);

    child->process_name = (char *) &p_name;
    for (int i = 0; i < BUF_SIZE; i++) child->cwd[i] = parent_task->cwd[i];

    file_object_t *stdout_fo = get_stdout_fo();
    stdout_fo->ref_count++;
    child->fds[1] = stdout_fo;
    child->fds[2] = stdout_fo;
    parent_task->num_child++;

    child->cr3 = cow_page_tables();
    set_new_cr3(child->cr3);

    child->process_mm = (struct mm_struct *) kmalloc(sizeof(struct mm_struct));
    memcpy(child->process_mm, parent_task->process_mm, sizeof(struct mm_struct));

    struct vma_struct *p_vma = parent_task->process_mm->vma_map, *c_vma_map = NULL, *c_vma_iter = NULL;

    while (p_vma != NULL && p_vma != parent_task->process_mm->vma_heap) {
        struct vma_struct *c_vma = (struct vma_struct *) kmalloc(sizeof(struct vma_struct));
        memcpy(c_vma, p_vma, sizeof(struct vma_struct));
        c_vma->next = NULL;
        if (c_vma_iter != NULL) {
            c_vma_iter->next = c_vma;
        } else {
            c_vma_map = c_vma;
        }
        c_vma_iter = c_vma;
        p_vma = p_vma->next;
    }

    child->process_mm->vma_map = c_vma_map;
    child->process_mm->vma_map_iter = c_vma_iter;

    struct vma_struct *p_vma_heap = parent_task->process_mm->vma_heap, *c_vma_heap = NULL, *c_vma_heap_iter = NULL;

    while (p_vma_heap != NULL) {
        struct vma_struct *c_vma = (struct vma_struct *) kmalloc(sizeof(struct vma_struct));
        c_vma->next = NULL;
        memcpy(c_vma, p_vma_heap, sizeof(struct vma_struct));

        if (c_vma_heap_iter != NULL) {
            c_vma_heap_iter->next = c_vma;
        } else {
            c_vma_heap = c_vma;
        }
        c_vma_heap_iter = c_vma;
        p_vma_heap = p_vma_heap->next;
    }

    child->process_mm->vma_heap = c_vma_heap;

    struct vma_struct *p_vma_stack = parent_task->process_mm->vma_stack;
    struct vma_struct *c_vma_stack = (struct vma_struct *) kmalloc(sizeof(struct vma_struct));

    memcpy(c_vma_stack, p_vma_stack, sizeof(struct vma_struct));
    child->process_mm->vma_stack = c_vma_stack;

    return (uint64_t) child;
}

void fork() {
    uint64_t return_val = (uint64_t) __builtin_return_address(0);
    kthread_t *parent_task = current_process, *last;
    kthread_t *child_task = (kthread_t *) copy_process(parent_task);

    last = current_process->next;
    current_process->next = child_task;
    child_task->next = last;
    child_task->state = QUEUED;
    set_new_cr3(parent_task->cr3);

    for (int i = 0; i < 4096; i++) {
        *(child_task->k_stack + i) = *(parent_task->k_stack + i);
    }

    volatile uint64_t *var = (uint64_t * )(get_rsp_val() + 8);
    parent_task->rsp_val = (uint64_t * )(get_rsp_val() + 8);
    volatile uint64_t diff = parent_task->k_stack + K_STACK_SIZE - 1 - (var);
    child_task->rsp_val = (uint64_t * )(((uint64_t) child_task->rsp_val) - 8 * ((uint64_t) diff));
    while (*(child_task)->rsp_val != return_val) {
        child_task->rsp_val++;
    }
    put_in_rax((uint64_t) 0);
    child_task->rsp_val--;
    setup_forked_kthread_stack(child_task->rsp_val);
    child_task->rsp_val -= 15;
    put_in_rax((uint64_t) child_task->pid);
}

void get_process_state(char *buf) {
    kthread_t *start = get_current_process();
    kthread_t *it = start;
    int i = 0;
    char *s = buf;
    do {
        strcat(s, "Pid: ");
        char num[10];
        itoa(num, it->pid);
        strcat(s, num);
        strcat(s, ", ppid: ");
        memset(num, '\0', 10);
        itoa(num, it->ppid);
        strcat(s, num);
        strcat(s, ", name: ");
        strcat(s, it->process_name);
        strcat(s, ", status: ");
        char *stat_str = "";
        switch (it->state) {
            case RUNNING:
                stat_str = "Running";
                break;
            case QUEUED:
                stat_str = "Queued";
                break;
            case ZOMBIE:
                stat_str = "Zombie";
                break;
        }
        strcat(s, stat_str);
        strcat(s, "\n");
        it = it->next;
        i++;
    } while (it != start);
}

int exec_vpe(char *filename, char **argv, char **envp) {
    if(get_file_binary(filename) == NULL) {
        return -1;
    }
    char filename_copy[BUF_SIZE];
    memset((void *) filename_copy, 0, BUF_SIZE);
    for (int k = 0; filename[k] != 0; k++) {
        filename_copy[k] = filename[k];
    }
    uint64_t argc = 0;
    char user_stack[MAX_ARGS][BUF_SIZE];
    memset((void *) user_stack, 0, MAX_ARGS * BUF_SIZE);
    if (argv != NULL) {
        for (int i = 0; argv[i] != NULL; i++, argc++) {
            for (int j = 0; argv[i][j] != 0; j++) {
                user_stack[argc][j] = argv[i][j];
            }
        }
    }

    kthread_t *new_process = (kthread_t *) kmalloc(sizeof(kthread_t));

    new_process->pid = current_process->pid;
    new_process->ppid = current_process->ppid;
    new_process->next = NULL;
    new_process->num_child = 0;

    char p_name[BUF_SIZE];
    p_name[0] = 0;
    strcat((char *) p_name, filename);

    new_process->process_name = (char *) &p_name;

    new_process->fds[0] = current_process->fds[0];
    new_process->fds[1] = current_process->fds[1];
    new_process->fds[2] = current_process->fds[2];

    for (int i = 0; i < BUF_SIZE; i++) {
        new_process->cwd[i] = current_process->cwd[i];
    }

    new_process->cr3 = setup_user_page_tables();
    set_new_cr3(new_process->cr3);

    memset(new_process->k_stack, 0, K_STACK_SIZE);
    new_process->rsp_val = &(new_process->k_stack[K_STACK_SIZE - 17]);

    struct mm_struct *process_mm = (struct mm_struct *) kmalloc(sizeof(struct mm_struct));
    new_process->process_mm = process_mm;

    load_file(new_process, filename_copy);

    void *user_stack_ptr = (void *) (STACK_START + 4096 - 16 - sizeof(user_stack));
    memcpy(user_stack_ptr, (void *) user_stack, sizeof(user_stack));

    for (int i = argc; i > 0; i--) {
        *((uint64_t * )(user_stack_ptr - 8 * i)) = (uint64_t) user_stack_ptr + BUF_SIZE * (argc - i);
    }
    user_stack_ptr = user_stack_ptr - 8 * argc;
    new_process->rsp_user = (uint64_t) user_stack_ptr;

    kthread_t *last = current_process->next;
    while (last->next != current_process) {
        last = last->next;
    }
    last->next = new_process;
    new_process->next = current_process->next;
    // TODO free and cleanup current process here.
    current_process = new_process;
    current_process->state = RUNNING;
    go_to_ring3_exec(argc, user_stack_ptr);
    return -1;
}

void go_to_ring3_exec(uint64_t argc, void *user_stack_ptr) {
    set_tss_rsp(&current_process->k_stack[K_STACK_SIZE - 1]);
    set_new_cr3(current_process->cr3);

    __asm__ __volatile__ (
    "sti;"
            "movq %0, %%rsp;"
            "movq %2, %%rax;"
            "pushq $0x23;"
            "pushq %%rax;"
            "pushfq;"
            "pushq $0x2B;"
            "pushq %1;"
            "movq %3, %%rsi;"
            "movq %4, %%rdi;"
            "iretq;"
    ::"r"(current_process->rsp_val), "r"(current_process->rip), "r"(current_process->rsp_user), "r"(user_stack_ptr), "r"(argc)
    );
}

void deep_cleanup(kthread_t *process) {

}

// clean pages, clean vma, clean mm_struct
void shallow_cleanup(kthread_t *process) {
    struct vma_struct *v_map = process->process_mm->vma_map;
    struct vma_struct *v_stack = process->process_mm->vma_stack;
    struct vma_struct *v_map_next = v_map;
    struct vma_struct *prev = NULL;
    while (v_map_next != NULL) {
        uint64_t s_addr = v_map_next->start;
        while (s_addr < v_map_next->end) {
            kfree((void *) s_addr);
            s_addr += PAGE_SIZE;
        }
        prev = v_map_next;
        v_map_next = v_map_next->next;
        kfree(prev);
    }
    uint64_t stack_end = v_stack->end;
    while (stack_end < v_stack->start) {
        kfree((void *) stack_end);
        stack_end += PAGE_SIZE;
    }
    kfree(v_stack);
    kfree(process->process_mm);
}

void reap_process(kthread_t *process) {
    kthread_t *cur = current_process;
    if (process == cur) {
        kprintf("Can't reap the only process!\n");
        return;
    }
    kthread_t *prev = cur;
    kthread_t *it = cur->next;
    while (it != process) {
        prev = it;
        it = it->next;
    }
    prev->next = process->next;
    deep_cleanup(process);
    processes[process->pid] = 0;
    kfree(process);
    // remove from list
    // deep clean
    // free the data structure
}

void kill_process(kthread_t *process) {
    kthread_t *cur = process;
    if (cur->next == cur) {
        kprintf("Can't exit the only process!\n");
        return;
    }
    // cleanup
    kthread_t *it = cur;
    it = it->next;
    while (it != cur) {
        if (it->ppid == cur->pid) {
            // this is my child. change it's ppid to 1
            it->ppid = 1;
        }
        it = it->next;
    }
    current_process->state = ZOMBIE;
    shallow_cleanup(current_process);
    // cleanup happens in page tables
}

int kill_kern(int pid) {
    if (pid <= 0) {
        kprintf("Invalid pid <= 0\n");
        return -1;
    }
    if (pid >= MAX_P) {
        kprintf("Pid %d out of range\n", pid);
        return -1;
    }

    if (pid == 1) {
        kprintf("Can't kill pid 1: kernel idle process\n");
        return -1;
    }
    if (processes[pid] == 0) {
        kprintf("No process exists with pid: %d\n", pid);
        return -1;
    }
    if (current_process == current_process->next) {
        if (current_process->pid == pid) {
            kprintf("Can't kill the only process! pid: %d\n", pid);
            return -1;
        }
    }
    kthread_t *it = current_process->next;
    while (it != current_process) {
        if (it->pid == pid) {
            return -1;
        }
        it = it->next;
    }
    if (it->pid != pid) {
        kprintf("No process exists with pid: %d\n", pid);
        return -1;
    }
    kill_process(it);
    return 0;
}

void exit_current_process(int status) {
    kill_process(current_process);
    scheduler();
}