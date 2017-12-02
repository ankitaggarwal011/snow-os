#include <sys/defs.h>
#include <sys/gdt.h>
#include <sys/kprintf.h>
#include <sys/tarfs.h>
#include <sys/ahci.h>
#include <sys/pit.h>
#include <sys/pic.h>
#include <sys/interrupt.h>
#include <sys/keyboard.h>
#include <sys/string.h>
#include <sys/physical_memory.h>
#include <sys/paging.h>
#include <sys/kthread.h>
#include <sys/elf64.h>
#include <sys/process.h>

#define INITIAL_STACK_SIZE 4096
uint8_t initial_stack[INITIAL_STACK_SIZE]__attribute__((aligned(16)));
uint32_t *loader_stack;
extern char kernmem, physbase;

void start(uint32_t *modulep, void *physbase, void *physfree) {
    uint64_t base = 0x0, length = 0;
    struct smap_t {
        uint64_t base, length;
        uint32_t type;
    }__attribute__((packed)) *smap;
    while (modulep[0] != 0x9001) modulep += modulep[1] + 2;
    for (smap = (struct smap_t *) (modulep + 2);
         smap < (struct smap_t *) ((char *) modulep + modulep[1] + 2 * 4); ++smap) {
        if (smap->type == 1 /* memory */ && smap->length != 0) {
            kprintf("Available Physical Memory [%p-%p]\n", smap->base, smap->base + smap->length);
            if (smap->base <= (uint64_t) physfree && smap->base + smap->length > (uint64_t) physfree) {
                base = smap->base;
                length = smap->length;
            }
        }
    }

    kprintf("physfree %p\n", (uint64_t) physfree);
    kprintf("tarfs in [%p:%p]\n", &_binary_tarfs_start, &_binary_tarfs_end);

    init_physical_memory((uint64_t) physfree, base, length);
    init_paging((uint64_t) & kernmem, (uint64_t) physbase, (uint64_t) physfree);

    kprintf("Paging works!\n");

    char *test = (char *) kmalloc(2);
    test[0] = 'a';
    test[1] = '\0';
    kprintf("Testing kmalloc: %s\n", test);
    kprintf("kmalloc works!\n");

    // test_context_switch();
    // init_tarfs();
    init_processes();
    init_idle_process();
    kthread_t *user_task = create_process("bin/sbush");
    // kprintf("User task RIP: %p\n", user_task->rip);
    go_to_ring3(user_task);
    while (1);
}

void boot(void) {
    // note: function changes rsp, local stack variables can't be practically used
    // register char *temp1, *temp2;
    // register char *temp2;
    // for (temp2 = (char *) 0xb8001; temp2 < (char *) 0xb8000 + 160 * 25; temp2 += 2) *temp2 = 7 /* white */;
    __asm__ volatile(
    "cli;"
            "movq %%rsp, %0;"
            "movq %1, %%rsp;"
    :"=g"(loader_stack)
    :"r"(&initial_stack[INITIAL_STACK_SIZE])
    );
    init_gdt();
    init_idt();
    init_pit();
    init_pic();
    resetVideoMemory(' ', 7);
    init_keyboard();

    start(
            (uint32_t * )((char *) (uint64_t) loader_stack[3] + (uint64_t) & kernmem - (uint64_t) & physbase),
            (uint64_t * ) & physbase,
            (uint64_t * )(uint64_t)
    loader_stack[4]
    );
}
