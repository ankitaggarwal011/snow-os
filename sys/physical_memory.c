#include <sys/defs.h>
#include <sys/kprintf.h>

#define PHYS_OFFSET 0x3E8000UL
#define PAGE_SIZE 4096
#define MIN_PAGES 10

typedef struct physical_page {
    uint64_t addr;
    uint64_t index;
    struct physical_page *next;
} physical_page;

physical_page *free_list;
uint64_t max_pages_available = 0, pages_used = 0, base_addr = 0x0, physical_mem_size = 0;

void init_physical_memory(uint64_t physfree, uint64_t base, uint64_t length) {
    uint64_t page_number = 0;
    base_addr = base + PHYS_OFFSET; // 4M offset for physical pages
    physical_mem_size = length - PHYS_OFFSET;
    max_pages_available = physical_mem_size / PAGE_SIZE; // 4k for each page
    memset((uint64_t *)base_addr, 0x0, physical_mem_size);
    for (uint64_t i = base_addr; i < physical_mem_size; i += PAGE_SIZE) {
        physical_page page = {base_addr, page_number, NULL};
        free_list->next = &page;
        page_number++;
    }
    kprintf("Base address: %p, memory size: %d, max pages available: %d\n", base_addr, physical_mem_size, max_pages_available);
}

uint64_t get_free_pages_count() {
    return max_pages_available - pages_used;
}

physical_page* get_free_page() {
    if (get_free_pages_count() < MIN_PAGES) {
        return NULL;
    }
    physical_page* free_page = free_list;
    free_list = free_list->next;
    free_page->next = NULL;
    pages_used++;
    return (uint64_t) free_page;
}

physical_page* get_free_pages(uint64_t num_of_pages) {
    if (get_free_pages_count() - num_of_pages < MIN_PAGES) {
        return NULL;
    }
    uint64_t pages = num_of_pages - 1;
    physical_page* free_pages = free_list;
    while (pages--) {
        free_list = free_list->next;
        pages_used++;
    }
    physical_page* free_page = free_list;
    free_list = free_list->next;
    free_page->next = NULL;
    pages_used++;
    return (uint64_t) free_pages;
}