#include <sys/defs.h>
#include <sys/kprintf.h>
#include <sys/physical_memory.h>
#include <sys/string.h>
#include <sys/tarfs.h>
#include <sys/elf64.h>
#include <sys/process.h>
#include <sys/kthread.h>
#include <sys/paging.h>

int getPID() {
    for(int i = 0; i < MAX_P; i++) {
        if(processes[i] == 0) {
            processes[i] = 1;
            return i;
        }
    }
    return -1;
}

void init_processes() {
    for(int i = 0; i < MAX_P; i++) {
        processes[i] = 0;
    }
}

kthread_t* create_process(char *filename) {
    kthread_t *new_process = (kthread_t *) kmalloc(sizeof(kthread_t *));
    new_process->pid = getPID();

    new_process->cr3 = (uint64_t) setup_user_page_tables();
    uint64_t current_cr3 = get_cr3();
    set_new_cr3(new_process->cr3);
    struct mm_struct *process_mm = (struct mm_struct *) kmalloc(sizeof(struct mm_struct));
    new_process->process_mm = process_mm;

    void *kernel_stack = (void *)kmalloc(4096);
    new_process->k_stack = ((uint64_t)kernel_stack + 4096 - 16);

    load_file(new_process, char* filename);

    set_new_cr3(current_cr3); 

    return new_process;
}