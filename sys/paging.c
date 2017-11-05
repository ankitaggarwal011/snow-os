#include <sys/defs.h>
#include <sys/kprintf.h>
#include <sys/physical_memory.h>

#define PAGE_SIZE 4096
#define PHYS_VIDEO_MEM 0xB8000
#define ENTRIES 512

uint64_t *pml4_t, kernel_virtual_base;

void setup_page_tables(uint64_t virtual, uint64_t physical) {
    // kprintf("Virtual: %x, Physical: %x\n", virtual, physical);
    uint64_t *pdpe_t = NULL, *pde_t = NULL, *pte_t = NULL;
    uint32_t offset_pte = 0x1FF & (virtual >> 12),
             offset_pde = 0x1FF & (virtual >> 21),
             offset_pdpe = 0x1FF & (virtual >> 30),
             offset_pml4 = 0x1FF & (virtual >> 39);
    if ((pml4_t[offset_pml4] & 1UL) != 1UL) { // it is not present
        pml4_t[offset_pml4] = get_free_page() | 3UL; // page is present and writable
    }
    pdpe_t = (uint64_t*) pml4_t[offset_pml4];
    if ((pdpe_t[offset_pdpe] & 1UL) != 1UL) {
        pdpe_t[offset_pdpe] = get_free_page() | 3UL;
    }
    pde_t = (uint64_t*) pdpe_t[offset_pdpe];
    if ((pde_t[offset_pde] & 1UL) != 1UL) {
        pde_t[offset_pde] = get_free_page() | 3UL;
    }
    pte_t = (uint64_t*) pde_t[offset_pde];
    pte_t[offset_pte] = physical | 3UL;
    // kprintf("PML4: %p, PDPE: %p, PDE: %p, PTE: %p, Physical: %p\n", pml4_t, pdpe_t, pde_t, pte_t, pte_t[offset_pte]);
}

void set_new_cr3(uint64_t cr3_addr) {
    __asm__ __volatile__ (
        "movq %0, %%cr3;" 
        :: 
        "=r"(cr3_addr)
    );
}

void init_paging(uint64_t kernmem, uint64_t physbase, uint64_t physfree) {
    uint64_t cr3_addr = get_free_page(), i = 0x0;
    kprintf("Kernel pages: %d\n", (physfree - physbase) / PAGE_SIZE);
    kernel_virtual_base = kernmem - physbase;
    pml4_t = (uint64_t *) cr3_addr;
    pml4_t[510] = cr3_addr | 3UL;
    while (physbase + i < physfree) {
        setup_page_tables(kernmem + i, physbase + i);
        i += PAGE_SIZE;
    }
    setup_page_tables(kernel_virtual_base + PHYS_VIDEO_MEM, PHYS_VIDEO_MEM);
    set_new_cr3(cr3_addr);
}
