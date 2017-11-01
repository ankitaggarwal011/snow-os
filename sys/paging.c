#include <sys/defs.h>
#include <sys/kprintf.h>
#include <sys/physical_memory.h>

#define PAGES_FOR_KERNEL 512

typedef struct physical_page {
    uint64_t addr;
    uint64_t index;
    struct physical_page *next;
} physical_page;

uint64_t cr3;

void init_paging(uint64_t kernmem, uint64_t physbase) {
    physical_page *page = (physical_page *) get_free_page();
    cr3 = page->addr;
}