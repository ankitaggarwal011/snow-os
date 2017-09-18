#include <sys/defs.h>
#include <sys/io.h>
#include <sys/kprintf.h>
#include <sys/pic.h>

void init_pic() {
    // Default initialization with 0x08 (master) and 0x70 (slave) offset
    // kprintf("Initialized PIC.\n");
}
