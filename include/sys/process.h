#ifndef _PROCESS_H
#define _PROCESS_H

#include <sys/defs.h>
#include <sys/kthread.h>

#define PAGE_SIZE 4096
#define HEAP_START 0x08000000
#define STACK_START 0xF0000000

#define TEXT 1
#define DATA 2
#define HEAP 3
#define STACK 4
#define UNDEFINED 5

#define PR 0x4
#define PW 0x2
#define PX 0x1

#define MAX_P 50

int processes[MAX_P];

void init_processes();

kthread_t *create_process(char *filename);

int getPID();

uint64_t copy_process(kthread_t *parent_task);

void fork();

kthread_t *init_idle_process();

void init_scheduler();

void scheduler();

void switch_process();

void go_to_ring3();

kthread_t *get_current_process();

int get_process_pid();

int get_process_ppid();

uint64_t user_malloc(uint64_t bytes);

void user_free(uint64_t addr);

int get_cwd(char *buf, size_t size);

int ch_dir(char *path);

void get_process_state(char *buf);

int exec_vpe(char *filename, char **argv, char **envp);

void go_to_ring3_exec();

void cleanup_process(kthread_t *process);

#endif