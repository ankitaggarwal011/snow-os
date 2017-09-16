#include <sys/defs.h>
#define memset __builtin_memset

struct idtr_struct {
    uint16_t limit;
    uint64_t offset;
};

struct idt_struct {
    uint16_t offset_1;
    uint16_t selector;
    uint8_t zero_1;
    uint8_t type_attr;
    uint16_t offset_2;
    uint32_t offset_3;
    uint32_t zero_2;
}

void init_idt();
void set_irq(uint8_t int_n, uint64_t addr, uint16_t selector, uint8_t type_attr);
void config_intr(uint8_t int_n, void* callback);
void itr_handler(uint8_t int_n);
