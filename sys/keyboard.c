#include <sys/defs.h>
#include <sys/io.h>
#include <sys/interrupt.h>
#include <sys/kprintf.h>
#include <sys/pit.h>
#include <sys/keyboard.h>
#include <sys/terminal.h>

uint8_t specialChars = 0x0;
//bit 5: Numlock
//bit 4: rntrl
//bit 3: lcntrl
//bit 1: rshift
//bit 1: lshift
//bit 0: caps: set to on alternate PRESSES.

uint8_t getScancode();

char getAsciiCode(uint8_t scanCode);

void printAscii(uint8_t sc);

typedef enum special_key {
    CAPS, LSHIFT, RSHIFT, LCNTRL, RCNTRL, NUMLOCK
} special_key;

extern void keyboard_intr_handler() {
    printAscii(getScancode());
    output_b(PIC_M_CR, PIC_EOI);
}

uint8_t getScancode() {
    return input_b(KB_CNTRL_ADDRESS);
}

int getBitmask(special_key sk) {
    switch (sk) {
        case CAPS:
            return 0b1;

        case LSHIFT:
            return 0b10;

        case RSHIFT:
            return 0b100;

        case LCNTRL:
            return 0b1000;

        case RCNTRL:
            return 0b10000;

        case NUMLOCK:
            return 0b100000;
    }
    return 0;
}

void clearKey(special_key sk) {
    specialChars = specialChars & ~(getBitmask(sk));
}

void setKey(special_key sk) {
    specialChars = specialChars | getBitmask(sk);
}

int isKey(special_key sk) {
    return specialChars & getBitmask(sk);
}

int isShiftEnabled() {
    return isKey(LSHIFT) | isKey(RSHIFT);
}

int shouldCapitalizeLetters() {
    return isKey(CAPS) ^ isShiftEnabled();
}

int handleNormalScanCodes(uint8_t sc) {
    char str[5];
    str[4] = '\0';
    char c;
    char p = '\0';
    char pp = '\0';
    switch (sc) {
        case 0x02:
            c = isShiftEnabled() ? '!' : '1';
            break;
        case 0x03:
            c = isShiftEnabled() ? '@' : '2';
            break;
        case 0x04:
            c = isShiftEnabled() ? '#' : '3';
            break;
        case 0x05:
            c = isShiftEnabled() ? '$' : '4';
            break;
        case 0x06:
            c = isShiftEnabled() ? '%' : '5';
            break;
        case 0x07:
            c = isShiftEnabled() ? '^' : '6';
            break;
        case 0x08:
            c = isShiftEnabled() ? '&' : '7';
            break;
        case 0x09:
            c = isShiftEnabled() ? '*' : '8';
            break;
        case 0x0A:
            c = isShiftEnabled() ? '(' : '9';
            break;
        case 0x0B:
            c = isShiftEnabled() ? ')' : '0';
            break;
        case 0x0C:
            c = isShiftEnabled() ? '_' : '-';
            break;
        case 0x0D:
            c = isShiftEnabled() ? '+' : '=';
            break;
        case 0x0E:
            c = 'H';
            p = '^';
            break;//ASCII for backspace
        case 0x0F:
            c = 't';
            p = '\\';
            break;//ASCII for tab
        case 0x10:
            c = shouldCapitalizeLetters() ? 'Q' : 'q';
            break;
        case 0x11:
            c = shouldCapitalizeLetters() ? 'W' : 'w';
            break;

        case 0x12:
            c = shouldCapitalizeLetters() ? 'E' : 'e';
            break;

        case 0x13:
            c = shouldCapitalizeLetters() ? 'R' : 'r';
            break;

        case 0x14:
            c = shouldCapitalizeLetters() ? 'T' : 't';
            break;

        case 0x15:
            c = shouldCapitalizeLetters() ? 'Y' : 'y';
            break;

        case 0x16:
            c = shouldCapitalizeLetters() ? 'U' : 'u';
            break;

        case 0x17:
            c = shouldCapitalizeLetters() ? 'I' : 'i';
            break;

        case 0x18:
            c = shouldCapitalizeLetters() ? 'O' : 'o';
            break;
        case 0x19:
            c = shouldCapitalizeLetters() ? 'P' : 'p';
            break;
        case 0x1A:
            c = isShiftEnabled() ? '{' : '[';
            break;
        case 0x1B:
            c = isShiftEnabled() ? '}' : ']';
            break;
        case 0x1C:
            //ENTER
            c = 'r';
            p = '\\';
            break;
        case 0x1E:
            c = shouldCapitalizeLetters() ? 'A' : 'a';
            break;
        case 0x1F:
            c = shouldCapitalizeLetters() ? 'S' : 's';
            break;
        case 0x20:
            c = shouldCapitalizeLetters() ? 'D' : 'd';
            break;
        case 0x21:
            c = shouldCapitalizeLetters() ? 'F' : 'f';
            break;
        case 0x22:
            c = shouldCapitalizeLetters() ? 'G' : 'g';
            break;
        case 0x23:
            c = shouldCapitalizeLetters() ? 'H' : 'h';
            break;
        case 0x24:
            c = shouldCapitalizeLetters() ? 'J' : 'j';
            break;
        case 0x25:
            c = shouldCapitalizeLetters() ? 'K' : 'k';
            break;
        case 0x26:
            c = shouldCapitalizeLetters() ? 'L' : 'l';
            break;
        case 0x27:
            c = isShiftEnabled() ? ':' : ';';
            break;
        case 0x28:
            c = isShiftEnabled() ? '\'' : '\"';
            break;
        case 0x29:
            c = isShiftEnabled() ? '~' : '`';
            break;
        case 0x2B:
            c = isShiftEnabled() ? '|' : '\\';
            break;
        case 0x2C:
            c = shouldCapitalizeLetters() ? 'Z' : 'z';
            break;
        case 0x2D:
            c = shouldCapitalizeLetters() ? 'X' : 'x';
            break;
        case 0x2E:
            c = shouldCapitalizeLetters() ? 'C' : 'c';
            break;
        case 0x2F:
            c = shouldCapitalizeLetters() ? 'V' : 'v';
            break;
        case 0x30:
            c = shouldCapitalizeLetters() ? 'B' : 'b';
            break;
        case 0x31:
            c = shouldCapitalizeLetters() ? 'N' : 'n';
            break;
        case 0x32:
            c = shouldCapitalizeLetters() ? 'M' : 'm';
            break;
        case 0x33:
            c = isShiftEnabled() ? '<' : ',';
            break;
        case 0x34:
            c = isShiftEnabled() ? '>' : '.';
            break;
        case 0x35:
            c = isShiftEnabled() ? '?' : '/';
            break;
        case 0x37:
            //keypad star
            c = isKey(NUMLOCK) ? '*' : '\0';
            break;
        case 0x39:
            c = ' ';
            break;
        case 0x3B:
            c = '1';
            p = 'F';
            break;
        case 0x3C:
            c = '2';
            p = 'F';
            break;
        case 0x3D:
            c = '3';
            p = 'F';
            break;
        case 0x3E:
            c = '4';
            p = 'F';
            break;
        case 0x3F:
            c = '5';
            p = 'F';
            break;
        case 0x40:
            c = '6';
            p = 'F';
            break;
        case 0x41:
            c = '7';
            p = 'F';
            break;
        case 0x42:
            c = '8';
            p = 'F';
            break;
        case 0x43:
            c = '9';
            p = 'F';
            break;
        case 0x44:
            c = '0';
            p = '1';
            pp = 'F';
            break;
        case 0x47:
            c = isKey(NUMLOCK) ? '7' : '\0';
            break;
        case 0x48:
            c = isKey(NUMLOCK) ? '8' : '\0';
            break;
        case 0x49:
            c = isKey(NUMLOCK) ? '9' : '\0';
            break;
        case 0x4A:
            c = isKey(NUMLOCK) ? '-' : '\0';
            break;
        case 0x4B:
            c = isKey(NUMLOCK) ? '4' : '\0';
            break;
        case 0x4C:
            c = isKey(NUMLOCK) ? '5' : '\0';
            break;
        case 0x4D:
            c = isKey(NUMLOCK) ? '6' : '\0';
            break;
        case 0x4E:
            c = isKey(NUMLOCK) ? '+' : '\0';
            break;
        case 0x4F:
            c = isKey(NUMLOCK) ? '1' : '\0';
            break;
        case 0x50:
            c = isKey(NUMLOCK) ? '2' : '\0';
            break;
        case 0x51:
            c = isKey(NUMLOCK) ? '3' : '\0';
            break;
        case 0x52:
            c = isKey(NUMLOCK) ? '0' : '\0';
            break;
        case 0x53:
            c = isKey(NUMLOCK) ? '.' : '\0';
            break;
        case 0x57:
            c = '1';
            p = '1';
            pp = 'F';
            break;
        case 0x58:
            c = '2';
            p = '1';
            pp = 'F';
            break;
        default:
            c = '\0';
    }
    if (c == '\0') {
        return 0;
    }
    str[3] = c;
    int i = 2;

    if (p != '\0') {
        str[i--] = p;
    }
    if (pp != '\0') {
        str[i--] = pp;
    }
    if ((isKey(RCNTRL) | isKey(LCNTRL)) && str[i + 1] != '^') {
        str[i] = '^';
        printInputChar(str + i);
    } else {
        printInputChar(str + i + 1);
        // no need to insert extra char
    }
    return 1;
}

void printAscii(uint8_t sc) {
    // handle special chars first
    switch (sc) {
        case 0x2A:
            setKey(LSHIFT);
            return;

        case 0xAA:
            clearKey(LSHIFT);
            return;

        case 0x36:
            setKey(RSHIFT);
            return;

        case 0xB6:
            clearKey(RSHIFT);
            return;

        case 0x1D:
            setKey(LCNTRL);
            setKey(RCNTRL);
            return;

        case 0x9D:
            clearKey(LCNTRL);
            clearKey(RCNTRL);
            return;

        case 0x3A:
            //handle only capsLock press. No release handling.
            if (isKey(CAPS)) {
                clearKey(CAPS);
            } else {
                setKey(CAPS);
            }
            return;

        case 0x45:
            if (isKey(NUMLOCK)) {
                clearKey(NUMLOCK);
            } else {
                setKey(NUMLOCK);
            }
            return;
    }
        handleNormalScanCodes(sc);

}


void init_keyboard() {
//    disable_interrupts();
    //test PS2 controller
//    output_b(0x64, 0xAA);
//    while (!input_b(0x64 & 0b1));
//    uint8_t controller_status = input_b(0x60);
//    if (controller_status != 0x55) {
//        kprintf("KB Cntrl faulty. Status: %d\n", controller_status);
//        return;
//    }
//    //test 1st PS/2 port for keyboard
//    output_b(0x64, 0xAB);
//    while (!input_b(0x64 & 0b1));
//    uint8_t port_status = input_b(0x60);
//    if (port_status != 0x00) {
//        kprintf("KB Port faulty. Status: %d\n", controller_status);
//        return;
//    }
//    enable_interrupts();
}