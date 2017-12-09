#include <sys/defs.h>
#include <sys/kprintf.h>

#define PAGE_SIZE 4096
#define MIN_PAGES 10
#define MAX_PAGES 100000

struct physical_page {
    uint64_t page_number;
    struct physical_page *next;
    uint32_t ref_count;
}__attribute__((__packed__));

struct physical_page *free_list;
struct physical_page page_descriptor[MAX_PAGES];
uint64_t max_pages_available = 0, max_free_pages_available = 0, pages_used = 0, base_addr = 0x0, physical_mem_size = 0, free_page;

void update_max_pages(uint32_t add_pages) {
    max_free_pages_available += add_pages;
    max_pages_available += add_pages;
}

uint32_t get_page_ref_count(uint64_t page_addr) {
    uint64_t page_number = page_addr / PAGE_SIZE;
    return page_descriptor[page_number].ref_count;
}

void set_page_ref_count(uint64_t page_addr, uint32_t new_ref_count) {
    uint64_t page_number = page_addr / PAGE_SIZE;
    page_descriptor[page_number].ref_count = new_ref_count;
}

void init_physical_memory(uint64_t physfree, uint64_t base, uint64_t length) {
    physical_mem_size = base + length;
    max_free_pages_available = (physical_mem_size - physfree) / PAGE_SIZE; // 4k for each page
    max_pages_available = physical_mem_size / PAGE_SIZE;
    memset((uint64_t *) physfree, (uint64_t) 0x0, (max_free_pages_available * PAGE_SIZE) / 8);
    for (uint64_t pg = 0; pg < max_pages_available; pg++) {
        page_descriptor[pg].page_number = pg;
        page_descriptor[pg].ref_count = 0;
        if (pg < max_pages_available - 1) {
            page_descriptor[pg].next = &page_descriptor[pg + 1];
        }
        if (pg * PAGE_SIZE == physfree) {
            free_list = &page_descriptor[pg];
        }
    }
    // kprintf("Base address: %p, memory size: %d, max pages available: %d, max free pages available: %d\n", base_addr, physical_mem_size, max_pages_available, max_free_pages_available);
    // kprintf("Free list starts from: %p\n", free_list->page_number * PAGE_SIZE);
}

uint64_t get_free_pages_count() {
    return max_free_pages_available - pages_used;
}

uint64_t get_free_page() {
    if (get_free_pages_count() < MIN_PAGES) {
        return 0x0;
    }
    free_page = free_list->page_number * PAGE_SIZE;
    set_page_ref_count(free_page, 1);
    free_list = free_list->next;
    pages_used++;
    // kprintf("After allocating new page, free list starts from: %p\n", free_list->page_number * PAGE_SIZE);
    return free_page;
}

uint64_t get_free_pages(uint64_t num_of_pages) {
    if (get_free_pages_count() - num_of_pages < MIN_PAGES) {
        return 0x0;
    }
    free_page = free_list->page_number * PAGE_SIZE;
    free_list = &page_descriptor[free_list->page_number + num_of_pages];
    pages_used += num_of_pages;
    return free_page;
}

void add_back_free_pages(uint64_t page_addr, uint64_t num_of_pages) {
    uint64_t page_number = page_addr / PAGE_SIZE;
    struct physical_page *prev_free_list = free_list;
    free_list = &page_descriptor[page_number];
    for (uint64_t i = 0; i < num_of_pages; i++) {
        page_descriptor[page_number + i].ref_count = 0;
    }
    page_descriptor[page_number + num_of_pages - 1].next = prev_free_list;
    pages_used -= num_of_pages;
    // kprintf("After adding the page back, free list starts from: %p\n", free_list->page_number * PAGE_SIZE);
}
