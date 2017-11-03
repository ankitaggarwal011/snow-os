#include <sys/defs.h>
#include <sys/kprintf.h>
#include <sys/physical_memory.h>

#define PAGE_SIZE 4096
#define PHYS_VIDEO_MEM 0xB8000
#define ENTRIES 512

uint64_t cr3, *pml4_t;
uint64_t *pdpe_t = NULL, *pde_t = NULL, *pte_t = NULL;

void setup_page_tables(uint64_t virtual) {
    uint32_t offset_pdpe = 0x1FF & (virtual >> 12),
             offset_pde = 0x1FF & (virtual >> 21),
             // offset_pte = 0x1FF & (virtual >> 30),
             offset_pml4 = 0x1FF & (virtual >> 39);
    uint64_t pdpe = get_free_page();
    uint64_t pde = get_free_page();
    uint64_t pte = get_free_page();
    pml4_t[offset_pml4] = pdpe | 3; // page is present and writable
    pdpe_t = (uint64_t*) (virtual + pdpe); // virtual is kernmem
    pdpe_t[offset_pdpe] = pde | 3; // page is present and writable  
    pde_t = (uint64_t*) (virtual + pde);
    pde_t[offset_pde] = pte | 3; // page is present and writable   
    pte_t = (uint64_t*) (virtual + pte);
}

void map_video_memory(uint64_t virtual, uint64_t physical) {
    uint32_t offset_pte = 0x1FF & (virtual >> 30);
    for (uint32_t i = offset_pte; i < (offset_pte + 1); i++) { // only 1 page
        pte_t[i] = physical | 3; 
        physical += PAGE_SIZE;
    }
}

void map_virtual_to_physical(uint64_t virtual, uint64_t physical, uint64_t pages, uint64_t kernmem) {
    uint32_t // offset_pdpe = 0x1FF & (virtual >> 12),
             offset_pde = 0x1FF & (virtual >> 21),
             offset_pte = 0x1FF & (virtual >> 30);
             //offset_pml4 = 0x1FF & (virtual >> 39);
    uint32_t number_of_pte_t, j;
    for (uint32_t i = offset_pte ; i < ENTRIES; i++) {
        pte_t[i] = physical | 3;
        physical += PAGE_SIZE;
    }
    pages = pages - (ENTRIES - offset_pte);
    number_of_pte_t = pages / ENTRIES;
    for (j = 1; j <= number_of_pte_t; j++) {
        uint64_t pte = get_free_page();
        pde_t[offset_pde + j] = pte | 3; // page is present and writable
        pte_t = (uint64_t*) (kernmem + pte);
        for(uint32_t k = 0; k < ENTRIES; k++ ) { 
            pte_t[k] = physical | 3;
            physical += PAGE_SIZE;
        }
    }
    pages = pages - (ENTRIES * offset_pte);
    uint64_t pte = get_free_page();
    pde_t[offset_pde + j] = pte | 3; // page is present and writable 
    pte_t = (uint64_t*) (kernmem + pte);
    for(uint32_t k = 0; k < pages; k++ ) { 
        pte_t[k] = physical | 3;
        physical += PAGE_SIZE;
    }
}

void _cr3() {
    __asm__ __volatile__ (
        "movq %0, %%cr3;" 
        :: 
        "r"(cr3)
    );
}

void init_paging(uint64_t kernmem, uint64_t physbase, uint64_t physfree) {
    uint64_t kernel_pages = (physfree - physbase) / PAGE_SIZE;
    // kprintf("Kernel pages: %d\n", kernel_pages);
    cr3 = get_free_page();
    pml4_t = (uint64_t *) (kernmem + cr3);
    pml4_t[510] = cr3 | 3; // page is present and writable
    setup_page_tables(kernmem);
    map_virtual_to_physical(kernmem, physbase, kernel_pages, kernmem + get_free_pages_count());
    map_video_memory(kernmem + PHYS_VIDEO_MEM, PHYS_VIDEO_MEM);
    _cr3(cr3);
}
