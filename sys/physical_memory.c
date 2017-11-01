#include <sys/defs.h>
#include <sys/kprintf.h>

#define PAGES_OFFSET 1000
#define PAGE_SIZE 4096
#define MIN_PAGES 10
#define MAX_PAGES 262144

struct physical_page {
    uint64_t page_number;
    struct physical_page *next;
}__attribute__((__packed__));

struct physical_page *free_list;
struct physical_page page_descriptor[MAX_PAGES];
uint64_t max_pages_available = 0, pages_used = 0, base_addr = 0x0, physical_mem_size = 0;

void init_physical_memory(uint64_t physfree, uint64_t base, uint64_t length) {
    physical_mem_size = base + length;
    max_pages_available = (physical_mem_size - physfree - PAGES_OFFSET * PAGE_SIZE) / PAGE_SIZE; // 4k for each page
    memset((uint64_t *)physfree, 0x0, (max_pages_available + PAGES_OFFSET) * PAGE_SIZE);
    for (uint64_t pg = 0; pg < max_pages_available; pg++) {
        page_descriptor[pg].page_number = pg;
        if (pg < max_pages_available - 1) {
            page_descriptor[pg].next = &page_descriptor[pg + 1];
        }
        if (pg * PAGE_SIZE == physfree + PAGES_OFFSET * PAGE_SIZE) {
            free_list = &page_descriptor[pg];
        }
    }
    kprintf("Base address: %p, memory size: %d, max pages available: %d\n", base_addr, physical_mem_size, max_pages_available);
    kprintf("Free list starts from: %p\n", free_list->page_number * PAGE_SIZE);
}

uint64_t get_free_pages_count() {
    return max_pages_available - pages_used;
}

uint64_t get_free_page() {
    if (get_free_pages_count() < MIN_PAGES) {
        return 0x0;
    }
    uint64_t free_page = free_list->page_number * PAGE_SIZE;
    free_list = free_list->next;
    pages_used++;
    return free_page;
}

uint64_t get_free_pages(uint64_t num_of_pages) {
    if (get_free_pages_count() - num_of_pages < MIN_PAGES) {
        return 0x0;
    }
    uint64_t free_pages = free_list->page_number * PAGE_SIZE;
    free_list = &page_descriptor[free_list->page_number + num_of_pages];
    pages_used += num_of_pages;
    return free_pages;
}

void add_back_free_pages(uint64_t page_number, uint64_t num_of_pages) {
    struct physical_page *prev_free_list = free_list;
    free_list = &page_descriptor[page_number];
    page_descriptor[page_number + num_of_pages].next = prev_free_list;
    pages_used -= num_of_pages;
}