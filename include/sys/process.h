#include <sys/defs.h>

#define PAGE_SIZE 4096
#define HEAP_START 0x08000000
#define STACK_START 0xF0000000

#define CODE 1
#define DATA 2
#define HEAP 3
#define STACK 4
#define UNDEFINED 5

#define PR 0x4
#define PW 0x2
#define PX 0x1

struct vma_struct {
    uint64_t start;
    uint64_t end;
    uint64_t type;
    uint64_t flags;
    struct vma_struct *next;    
};

struct mm_struct {
    struct vma_struct *vma_map;
    uint64_t s_data, e_data;
    uint64_t s_code, e_code;
    uint64_t stack, heap;
};