#include <sys/defs.h>
#include <sys/kprintf.h>
#include <sys/physical_memory.h>
#include <sys/string.h>
#include <sys/tarfs.h>
#include <sys/elf64.h>

void load_file(char *filename) {
    void *location = get_file_binary("bin/sbush");
    kprintf("Location of bin/sbush: %p\n", location);
    kprintf("Reading ELF64 header:\n");

    Elf64_Ehdr *ehdr = (Elf64_Ehdr *) location;
    Elf64_Phdr *phdr = (Elf64_Phdr *)((uint64_t)ehdr + ehdr->e_phoff);

    kprintf("RIP: %x\n", ehdr->e_entry);
    int i = 0;
    while (i < ehdr->e_phnum) {
        if (phdr->p_type == 1) {
            uint64_t vma_start = phdr->p_vaddr;
            uint64_t vma_end = phdr->p_vaddr + phdr->p_memsz;
            kprintf("VMA start: %x, VMA end: %x\n", vma_start, vma_end);
        }
        i++;
    }
}