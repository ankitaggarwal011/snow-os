#include <sys/defs.h>
#include <sys/kprintf.h>
#include <sys/physical_memory.h>

#define PAGE_SIZE 4096
#define PHYS_VIDEO_MEM 0xB8000
#define ENTRIES 512

uint64_t cr3_addr, *pml4_t;

void setup_page_tables(uint64_t virtual, uint64_t physical, uint64_t pages, uint64_t kernel_virtual_base) {
    kprintf("Virtual: %x, Physical: %x, Pages: %x, KERNEL BASE: %x\n", virtual, physical, pages, kernel_virtual_base);
    uint64_t *pdpe_t = NULL, *pde_t = NULL, *pte_t = NULL;
    uint32_t offset_pte = 0x1FF & (virtual >> 12),
             offset_pde = 0x1FF & (virtual >> 21),
             offset_pdpe = 0x1FF & (virtual >> 30),
             offset_pml4 = 0x1FF & (virtual >> 39);
    if ((pml4_t[offset_pml4] & 1) == 1) { // it is present
        pdpe_t = (uint64_t*) (kernel_virtual_base + pml4_t[offset_pml4]);
        if ((pdpe_t[offset_pdpe] & 1) == 1) {
            pde_t = (uint64_t*) (kernel_virtual_base + pdpe_t[offset_pdpe]);
            if ((pde_t[offset_pde] & 1) == 1) {
                pte_t = (uint64_t*) (kernel_virtual_base + pde_t[offset_pde]);
            }
            else {
                uint64_t pte = get_free_page();
                pde_t[offset_pde] = pte | 3;  
                pte_t = (uint64_t*) (kernel_virtual_base + pte);
            }
        }
        else {
            uint64_t pde = get_free_page();
            pdpe_t[offset_pdpe] = pde | 3;
            pde_t = (uint64_t*) (kernel_virtual_base + pde);

            uint64_t pte = get_free_page();
            pde_t[offset_pde] = pte | 3;  
            pte_t = (uint64_t*) (kernel_virtual_base + pte);
        }
    }
    else {
        uint64_t pdpe = get_free_page();
        pml4_t[offset_pml4] = pdpe | 3; // page is present and writable
        pdpe_t = (uint64_t*) (kernel_virtual_base + pdpe);

        uint64_t pde = get_free_page();
        pdpe_t[offset_pdpe] = pde | 3;
        pde_t = (uint64_t*) (kernel_virtual_base + pde);

        uint64_t pte = get_free_page();
        pde_t[offset_pde] = pte | 3;  
        pte_t = (uint64_t*) (kernel_virtual_base + pte);
    }

    uint32_t number_of_pte_t, j;
    if (pages + offset_pte <= ENTRIES) {
        for (uint64_t i = offset_pte; i < (offset_pte + pages); i++) {
            pte_t[i] = physical | 3; 
            physical += PAGE_SIZE;
        }
    }
    else {
        for (uint32_t i = offset_pte ; i < ENTRIES; i++) {
            pte_t[i] = physical | 3;
            physical += PAGE_SIZE;
        }
        pages = pages - (ENTRIES - offset_pte);
        number_of_pte_t = pages / ENTRIES;
        for (j = 1; j <= number_of_pte_t; j++) {
            uint64_t pte = get_free_page();
            pde_t[offset_pde + j] = pte | 3;
            pte_t = (uint64_t*) (kernel_virtual_base + pte);
            for(uint32_t k = 0; k < ENTRIES; k++ ) { 
                pte_t[k] = physical | 3;
                physical += PAGE_SIZE;
            }
        }
        pages = pages - (ENTRIES * offset_pte);
        uint64_t pte = get_free_page();
        pde_t[offset_pde + j] = pte | 3;
        pte_t = (uint64_t*) (kernel_virtual_base + pte);
        for(uint32_t k = 0; k < pages; k++ ) { 
            pte_t[k] = physical | 3;
            physical += PAGE_SIZE;
        }
    }
}

void _cr3() {
    __asm__ __volatile__ (
        "movq %0, %%cr3;" 
        :: 
        "r"(cr3_addr)
    );
}

void init_paging(uint64_t kernmem, uint64_t physbase, uint64_t physfree) {
    uint64_t kernel_pages = (physfree - physbase) / PAGE_SIZE;
    uint64_t kernel_virtual_base = kernmem - physbase;
    cr3_addr = get_free_page();
    pml4_t = (uint64_t *) (kernel_virtual_base + cr3_addr);
    pml4_t[510] = cr3_addr | 3;
    setup_page_tables(kernmem, physbase, kernel_pages, kernel_virtual_base);
    setup_page_tables(kernel_virtual_base + PHYS_VIDEO_MEM, PHYS_VIDEO_MEM, 1, kernel_virtual_base);
    _cr3();
}
