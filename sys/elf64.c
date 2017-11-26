#include <sys/defs.h>
#include <sys/kprintf.h>
#include <sys/physical_memory.h>
#include <sys/paging.h>
#include <sys/string.h>
#include <sys/tarfs.h>
#include <sys/elf64.h>
#include <sys/process.h>

void load_file(char *filename) {
    void *location = get_file_binary(filename);
    kprintf("Location of %s: %p\n", filename, location);
    kprintf("Reading ELF64 header:\n");

    struct mm_struct *process = kmalloc(sizeof(struct mm_struct));

    Elf64_Ehdr *ehdr = (Elf64_Ehdr *) location;
    Elf64_Phdr *phdr = (Elf64_Phdr *)((uint64_t)ehdr + ehdr->e_phoff);

    kprintf("Task RIP: %x\n", ehdr->e_entry);
    int i = 0;

    struct vma_struct *vma_map = NULL, *vma_map_iter = NULL;
    while (i < ehdr->e_phnum) {
        if (phdr->p_type == 1) {
            struct vma_struct *vma = (struct vma_struct*) kmalloc(sizeof(struct vma_struct));
            vma->start = phdr->p_vaddr;
            vma->end = phdr->p_vaddr + phdr->p_memsz;

            uint64_t pages = (((vma->end / PAGE_SIZE + 1) * PAGE_SIZE) - ((vma->start / PAGE_SIZE) * PAGE_SIZE)) / PAGE_SIZE;
            uint64_t v_addr = (phdr->p_vaddr / PAGE_SIZE) * PAGE_SIZE;
            while(pages--) {
                uint64_t page = get_free_page();
                update_page_tables(v_addr, page, PAGING_USER_R_W_FLAGS);
                v_addr += PAGE_SIZE;
            }

            vma->flags = phdr->p_flags;
            vma->type = UNDEFINED;
            vma->next = NULL;
            if (vma_map_iter != NULL) {
                vma_map_iter->next = vma;
            }
            else {
                vma_map = vma;
            }
            vma_map_iter = vma;
        }
        phdr++;
        i++;
    }
    struct vma_struct *vma_heap = (struct vma_struct*) kmalloc(sizeof(struct vma_struct));
    vma_heap->start = HEAP_START;
    vma_heap->end = HEAP_START + PAGE_SIZE;
    vma_heap->type = HEAP;
    vma_heap->flags = PR | PW;
    vma_heap->next = NULL;
    if (vma_map_iter != NULL) {
        vma_map_iter->next = vma_heap;
    }
    else {
        vma_map = vma_heap;
    }
    vma_map_iter = vma_heap;
    struct vma_struct *vma_stack = (struct vma_struct*) kmalloc(sizeof(struct vma_struct));
    vma_stack->start = STACK_START + PAGE_SIZE;
    vma_stack->end = STACK_START;
    vma_stack->type = STACK;
    vma_stack->flags = PR | PW;
    vma_stack->next = NULL;
    if (vma_map_iter != NULL) {
        vma_map_iter->next = vma_stack;
    }
    vma_map_iter = vma_stack;

    process->vma_map = vma_map;
    struct vma_struct *test = process->vma_map;
    kprintf("VMAs found: \n");
    while (test) {
        kprintf("VMA start: %x, VMA end: %x\n", test->start, test->end);
        test = test->next;
    }
}