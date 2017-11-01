#include <sys/defs.h>
#include <sys/kprintf.h>
#include <sys/physical_memory.h>

#define PAGES_FOR_KERNEL 512

uint64_t cr3;

void init_paging(uint64_t kernmem, uint64_t physbase) {
    cr3 = get_free_page();
}