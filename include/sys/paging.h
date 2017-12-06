#ifndef _PAGING_H
#define _PAGING_H

#include <sys/defs.h>

#define PAGING_KERNEL_R_W_FLAGS 0x3 // change to 0x3. 0x7 value is a hack for now
#define PAGING_USER_R_W_FLAGS 0x7

void init_paging(uint64_t kernmem, uint64_t physbase, uint64_t physfree);

void update_page_tables(uint64_t virt_addr, uint64_t physical_addr, uint16_t flags);

void set_new_cr3(uint64_t cr3_addr);

void *kmalloc(uint32_t bytes_required);

void kfree(void *ptr);

uint64_t get_cr3();

void flush_tlb();

uint64_t setup_user_page_tables();

uint64_t walk_page_table(uint64_t virt_addr);

uint64_t get_flags(uint64_t virt_addr);

void remove_page_table_mapping(uint64_t virt_addr);

uint64_t cow_page_tables();

#endif