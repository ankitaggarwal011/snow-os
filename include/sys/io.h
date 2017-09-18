#include <sys/defs.h>

#define PIT_0 0x40
#define PIT_CR (PIT_0 + 3)

#define PIC_M 0x20
#define PIC_S 0xA0
#define PIC_M_CR PIC_M
#define PIC_M_DATA (PIC_M + 1)
#define PIC_S_CR PIC_S
#define PIC_S_DATA (PIC_S + 1)
#define PIC_EOI 0x20

#define IRQ0 32

void output_b(uint16_t p, uint8_t v);
uint8_t input_b(uint16_t p);
