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
kthread_t* create_process(char *filename);
int getPID();

#endif