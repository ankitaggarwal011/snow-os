#include <stdio.h>
#include <string.h>
#include <sys/stdarg.h>

typedef enum format_type {
    STRING,
    INT,
    HEX,
    VOID,
    CHAR
} format_type;

uint64_t currentRow = 0;
uint64_t currentColumn = 0;
va_list args;

void printChar(char c) {
    putchar((int) c);
}

void printString(char *c) {
    puts(c);
}

uint64_t mod(uint64_t x) {
    return x > 0 ? x : -x;
}

void printLong(uint64_t x) {
    char buf[66];
    int i = 65;
    buf[i--] = '\0';
    uint64_t numCopy = mod(x);
    do {
        buf[i--] = 48 + numCopy % 10;
        numCopy /= 10;
    } while (numCopy);
    if (x < 0) {
        buf[i] = '-';
    } else {
        i++;
    }
    printString(buf + i);
}

void printHex(uint64_t x, int include_prefix) {
    char buf[19];
    int i = 18;
    buf[i--] = '\0';
    uint64_t numCopy = mod(x);
    do {
        int remain = numCopy % 16;
        if (remain < 10) {
            buf[i--] = 48 + remain;
        } else {
            buf[i--] = 65 + (remain - 10);
        }
        numCopy = numCopy >> 4;
    } while (numCopy);
    if (!include_prefix) {
        printString(buf + i + 1);
        return;
    }
    buf[i--] = 'x';
    buf[i] = '0';
    printString(buf + i);
}

void printSpecial(int argNumber, format_type ft) {
    if (ft == STRING) {
        printString(va_arg(args,
                           char * ));
    } else if (ft == INT) {
        printLong(va_arg(args,
                         int));
    } else if (ft == HEX) {
        printHex(va_arg(args,
                        uint64_t), 0);
    } else if (ft == VOID) {
        printHex(va_arg(args,
                        uint64_t), 1);
    } else if (ft == CHAR) {
        printChar(va_arg(args,
                         int));
    }
}

int printf(const char *fmt, ...) {
    va_start(args, fmt);
    const char *c = fmt;
    int argNumber = 0;
    while (*c != '\0') {
        if (*c == '%') {
            int handled = 1;
            //special character case
            switch (*(c + 1)) {
                case 'c':
                    printSpecial(argNumber++, CHAR);
                    break;

                case 's':
                    printSpecial(argNumber++, STRING);
                    break;
                case 'd':
                    printSpecial(argNumber++, INT);
                    break;
                case 'x':
                    printSpecial(argNumber++, HEX);
                    break;
                case 'p':
                    printSpecial(argNumber++, VOID);
                    break;
                default:
                    handled = 0;
            }
            if (handled) {
                c += 2;
            } else {
                c += 1;
            }
        } else {
            printChar(*c);
            c++;
        }
    }
    va_end(args);
    return 0;
}
