#include <sys/defs.h>

void output_b(uint16_t p, uint8_t v) {
    __asm__ volatile("outb %0, %1"
                    :
                    :"a"(v), "Nd"(p)
                    );
}

uint8_t input_b(uint16_t p) {
    uint8_t v;
    __asm__ volatile("inb %1, %0"
                    :"=a"(v)
                    :"Nd"(p)
                    );
    return v;
}

void config_intr(uint8_t intr, void* callback) {
    
}