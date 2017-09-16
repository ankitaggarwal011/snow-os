#include <sys/defs.h>
#include <sys/io.h>
#include <sys/kprintf.h>
#include <sys/interrupt.h>

// extern void* timer_isr;

struct idtr_struct idtr_t;
struct idt_struct idt_t[256];
// void* register_int[256];

void init_idt() {
    idtr_t.limit = sizeof(struct idt_struct) * 256 - 1;
    idtr_t.offset = (uint64_t) &idt_t;

    memset(&idt_t, 0, sizeof(struct idt_struct) * 256);
    set_irq(32, (uint64_t) timer_isr, 0x00, 0x8E);
}

void set_irq(uint8_t int_n, uint64_t addr, uint16_t selector, uint8_t type_attr) {
    idt_t[int_n].selector = selector;
    idt_t[int_n].type_attr = type_attr;
    uint16_t offset_1 = addr & 0xFFFF;
    uint16_t offset_2 = (addr >> 16) & 0xFFFF;
    uint32_t offset_3 = (addr >> 32) & 0xFFFFFFFF;
    idt_t[int_n].offset_1 = offset_1;
    idt_t[int_n].offset_2 = offset_2;
    idt_t[int_n].offset_3 = offset_3;
    idt_t[int_n].zero_1 = 0;
    idt_t[int_n].zero_2 = 0;
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