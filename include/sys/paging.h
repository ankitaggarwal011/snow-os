#include <sys/defs.h>

void init_paging(uint64_t kernmem, uint64_t physbase, uint64_t physfree);
void map_virtual_to_physical(uint64_t virtual, uint64_t physical, uint64_t pages);