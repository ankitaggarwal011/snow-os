#include <sys/defs.h>

void init_physical_memory(uint64_t physfree, uint64_t base, uint64_t length);
uint64_t get_free_pages_count();
uint64_t get_free_page();
uint64_t get_free_pages(uint64_t num_of_pages);
void add_back_free_pages(uint64_t page_addr, uint64_t num_of_pages);
void update_max_pages(uint32_t add_pages);