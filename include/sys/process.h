#include <sys/defs.h>

struct vma_struct {
    uint64_t start;
    uint64_t end;
    uint64_t type;
    uint64_t flags;
    vma_struct *next;    
};