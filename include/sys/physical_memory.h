#include <sys/defs.h>

typedef struct physical_page {
    uint64_t addr;
    uint64_t index;
    struct physical_page *next;
} physical_page;

void init_physical_memory(uint64_t physfree, uint64_t base, uint64_t length);
uint64_t get_free_pages_count();
uint64_t get_free_page();
uint64_t get_free_pages(uint64_t num_of_pages);