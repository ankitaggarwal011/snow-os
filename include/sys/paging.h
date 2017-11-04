#include <sys/defs.h>

void init_paging(uint64_t kernmem, uint64_t physbase, uint64_t physfree);
void setup_page_tables(uint64_t virtual, uint64_t physical);
void set_new_cr3(uint64_t cr3_addr);