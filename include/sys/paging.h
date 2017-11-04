#include <sys/defs.h>

void init_paging(uint64_t kernmem, uint64_t physbase, uint64_t physfree);
void setup_page_tables(uint64_t virtual, uint64_t physical, uint64_t pages, uint64_t kernel_virtual_base);
void _cr3();