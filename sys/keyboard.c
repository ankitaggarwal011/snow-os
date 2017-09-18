#include <sys/defs.h>
#include <sys/io.h>
#include <sys/interrupt.h>
#include <sys/kprintf.h>
#include <sys/pit.h>
#include <sys/keyboard.h>

char getScancode();

extern void keyboard_intr_handler() {
    char buf[2];
    buf[0] = getScancode();
    buf[1] = '\0';
    printInputChar(buf);
    output_b(PIC_M_CR, PIC_EOI);
}

char getScancode() {
    return (char) input_b(KB_CNTRL_ADDRESS);
}

void init_keyboard() {
//    disable_interrupts();
    //test PS2 controller
    output_b(0x64, 0xAA);
    while (!input_b(0x64 & 0b1));
    uint8_t controller_status = input_b(0x60);
    if (controller_status != 0x55) {
        kprintf("KB Cntrl faulty. Status: %d\n", controller_status);
        return;
    }
    //test 1st PS/2 port for keyboard
    output_b(0x64, 0xAB);
    while (!input_b(0x64 & 0b1));
    uint8_t port_status = input_b(0x60);
    if (port_status != 0x00) {
        kprintf("KB Port faulty. Status: %d\n", controller_status);
        return;
    }
//    enable_interrupts();
}