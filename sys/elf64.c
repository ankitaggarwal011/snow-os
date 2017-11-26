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

    Elf64_Ehdr *ehdr = (Elf64_Ehdr *) location;
    Elf64_Phdr *phdr = (Elf64_Phdr *)((uint64_t)ehdr + ehdr->e_phoff);

    kprintf("RIP: %x\n", ehdr->e_entry);
    int i = 0;
    struct vma_struct *vma_map = NULL;
    while (i < ehdr->e_phnum) {
        if (phdr->p_type == 1) {
            struct vma_struct *vma = (struct vma_struct*) kmalloc(sizeof(struct vma_struct));
            vma->start = phdr->p_vaddr;
            vma->end = phdr->p_vaddr + phdr->p_memsz;
            vma->flags = phdr->p_flags;
            vma->next = NULL;
            if (vma_map == NULL) {
                vma_map = vma;
            }
            else {
                vma_map->next = vma;
            }
        }
        phdr++;
        i++;
    }
    struct vma_struct *test;
    kprintf("VMAs found: \n");
    while (test) {
        kprintf("VMA start: %x, VMA end: %x\n", test->start, test->end);
        test = test->next;
    }
}