#include <sys/defs.h>

#define PHYS_OFFSET 0x3E8000UL
#define PAGE_SIZE 4096
#define MIN_PAGES 10

typedef struct physical_page {
    uint64_t addr;
    uint64_t index;
    struct physical_page *next;
}

physical_page *free_list;
uint64_t max_pages_available = 0, pages_used = 0, base_addr = 0x0, physical_mem_size = 0;

void init_physical_memory(uint64_t physfree, uint64_t base, uint64_t length);
uint64_t get_free_pages_count();
physical_page* get_free_page();
physical_page* get_free_pages(uint64_t num_of_pages);