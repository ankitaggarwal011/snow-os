#include <sys/defs.h>
#include <sys/kprintf.h>
#include <sys/physical_memory.h>

#define PAGE_SIZE 4096
#define PHYS_VIDEO_MEM 0xB8000
#define ENTRIES 512

uint64_t *pml4_t, kernel_virtual_base;

void setup_page_tables(uint64_t virtual, uint64_t physical) {
    uint64_t *pdpe_t = NULL, *pde_t = NULL, *pte_t = NULL;
    uint64_t pdpe, pde, pte;
    uint32_t offset_pte = 0x1FF & (virtual >> 12),
             offset_pde = 0x1FF & (virtual >> 21),
             offset_pdpe = 0x1FF & (virtual >> 30),
             offset_pml4 = 0x1FF & (virtual >> 39);
    
    if ((pml4_t[offset_pml4] & 1UL) != 1UL) {
        pdpe = get_free_page();
    }
    else {
        pdpe = pml4_t[offset_pml4] ^ 3UL;
    }
    pml4_t[offset_pml4] = pdpe | 3UL; // page is present and writable
    pdpe_t = (uint64_t *) (kernel_virtual_base + pdpe);

    if ((pdpe_t[offset_pdpe] & 1UL) != 1UL) {
        pde = get_free_page();
    }
    else {
        pde = pdpe_t[offset_pdpe] ^ 3UL;
    }
    pdpe_t[offset_pdpe] = pde | 3UL;
    pde_t = (uint64_t *) (kernel_virtual_base + pde);

    if ((pde_t[offset_pde] & 1UL) != 1UL) {
        pte = get_free_page();
    }
    else {
        pte = pde_t[offset_pde] ^ 3UL;
    }
    pde_t[offset_pde] = pte | 3UL;
    pte_t = (uint64_t *) (kernel_virtual_base + pte);
    pte_t[offset_pte] = physical | 3UL;
}

void set_new_cr3(uint64_t cr3_addr) {
    __asm__ __volatile__ ("movq %0, %%cr3;" :: "r"(cr3_addr));
}

void init_paging(uint64_t kernmem, uint64_t physbase, uint64_t physfree) {
    uint64_t cr3_addr = get_free_page(), v_i = kernmem, p_i = physbase;
    kernel_virtual_base = kernmem - physbase;
    pml4_t = (uint64_t *) (kernel_virtual_base + cr3_addr);
    while (p_i < (physfree + get_free_pages_count() * PAGE_SIZE)) { // 1:1 mapping
        setup_page_tables(v_i, p_i);
        v_i += PAGE_SIZE;
        p_i += PAGE_SIZE;
    }
    setup_page_tables(kernel_virtual_base + PHYS_VIDEO_MEM, PHYS_VIDEO_MEM);
    set_new_cr3(cr3_addr);
}
