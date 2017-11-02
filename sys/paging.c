#include <sys/defs.h>
#include <sys/kprintf.h>
#include <sys/physical_memory.h>

#define PAGE_SIZE 4096
#define PHYS_VIDEO_MEM 0xB8000

uint64_t cr3, *pml4_t, kernel_pages;

void map_virtual_to_physical(uint64_t virtual, uint64_t physical, uint64_t pages) {
    uint64_t *pdpe_t, *pde_t, *pte_t;
    uint32_t offset_pdpe = 0x1FF & (virtual >> 12),
             offset_pde = 0x1FF & (virtual >> 21),
             offset_pte = 0x1FF & (virtual >> 30),
             offset_pml4 = 0x1FF & (virtual >> 39);
    uint64_t pdpe = get_free_page();
    uint64_t pde = get_free_page();
    uint64_t pte = get_free_page();
    pml4_t[offset_pml4] = pdpe;   
    pdpe_t = (uint64_t*) (kernmem + pdpe);
    pdpe_t[offset_pdpe] = pde;   
    pde_t = (uint64_t*) (kernmem + pde);
    pde_t[offset_pde] = pte;   
    pte_t = (uint64_t*) (kernmem + pte);
}

void init_paging(uint64_t kernmem, uint64_t physbase, uint64_t physfree) {
    cr3 = get_free_page();
    kernel_pages = (physfree - physbase) / PAGE_SIZE;
    pml4_t = (uint64_t *) (kernmem + cr3);
    map_virtual_to_physical(physbase, kernmem, kernel_pages);
    map_virtual_to_physical(PHYS_VIDEO_MEM, kernmem + PHYS_VIDEO_MEM, 1);
    load_cr3(cr3);
}
