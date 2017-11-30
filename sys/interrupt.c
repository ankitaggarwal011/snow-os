#include <sys/defs.h>
#include <sys/io.h>
#include <sys/kprintf.h>
#include <sys/interrupt.h>

extern void timer_isr();
extern void keyboard_isr();
extern void _x86_load_idt();

struct idtr_struct idtr_t;
struct idt_struct idt_t[256];
// void* register_int[256];

void page_fault_handler() {
    volatile uint64_t addr;
    __asm__ volatile("mov %%cr2, %0" : "=r" (addr));
    kprintf("Page fault at %x\n", addr);
    while(1);
}

void init_idt() {
    idtr_t.limit = sizeof(struct idt_struct) * 256 - 1;
    idtr_t.offset = (uint64_t) idt_t;

    memset(&idt_t, 0, sizeof(struct idt_struct) * 256);
    set_irq(IRQ0, (uint64_t) timer_isr, (uint16_t) 0x8, 0x8E);
    set_irq(IRQ1, (uint64_t) keyboard_isr, (uint16_t) 0x8, 0x8E);
    set_irq(14, (uint64_t) page_fault_handler, (uint16_t) 0x8, 0x8E);
    _x86_load_idt((uint64_t) &idtr_t);
    // kprintf("Initialized IDT.\n");
}

void set_irq(uint8_t int_n, uint64_t addr, uint16_t selector, uint8_t type_attr) {
    idt_t[int_n].offset_1 = (uint16_t) addr & 0xFFFF;
    idt_t[int_n].selector = selector;
    idt_t[int_n].zero = 0;
    idt_t[int_n].type_attr = type_attr;
    idt_t[int_n].offset_2 = (uint16_t) ((addr >> 16) & 0xFFFF);
    idt_t[int_n].offset_3 = (uint32_t)((addr >> 32) & 0xFFFFFFFF);
    idt_t[int_n].reserved = 0;
}

/*
void config_intr(uint8_t int_n, void* callback) {
    register_int[int_n] = callback;
}


void itr_handler(uint8_t int_n) {
    kprintf("Interrupt received\n");
    void* callback = register_int[int_n];
    callback();
}
*/