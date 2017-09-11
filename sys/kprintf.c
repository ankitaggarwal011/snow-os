#include <sys/kprintf.h>
#include <sys/stdarg.h>

char* outputWriteAddress = (char*)0xb8000;
long totalCharsPrinted = 0;
va_list args;
typedef enum format_type {
    STRING,
    INT,
    HEX,
    VOID
} format_type;

void printSpecial(int argNumber, format_type ft);
void printChar(char c);
void printString(char* c);
void printHex(long x);
void printLong(long x);
void printVoid(void* v);

void kprintf(const char *fmt, ...) {
    va_start(args, fmt);
    const char* c = fmt;
    int specialCharCount = 0;
    int argNumber = 0;
    while (*c != '\0') {
        if (*c == '%') {
            //special character case
            switch (*(c + 1)) {
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
                    break;
            }
            specialCharCount++;
            c += 2;
        } else {
            printChar(*c);
            c++;
        }
    }
    va_end(args);
}

void printSpecial(int argNumber, format_type ft) {
    if (ft == STRING) {
        printString(va_arg(args, char*));
    }
    else if (ft == INT) {
        printLong(va_arg(args, int));
    }
    else if (ft == HEX) {
        printHex(va_arg(args, long));
    }
    else if (ft == VOID) {
        printVoid(va_arg(args, void*));
    }
}

void printChar(char c) {
    *outputWriteAddress = c;
    outputWriteAddress += 2;
    totalCharsPrinted++;
}


void printString(char *c) {
    while (*c != '\0') {
        printChar(*(c++));
    }
}

void printLong(long x) {
    char buf[66];
    int i = 65;
    buf[i--] = '\0';
    long numCopy = x;
    while(numCopy) {
        buf[i--] = 48 + numCopy % 10;
        numCopy /= 10;
    }
    if (x < 0) {
        buf[i] = '-';
        printString((char*) (buf) + i);
    }
    else {
        printString((char*) (buf) + i + 1);
    }
}

void printHex(long x) {
    char buf[19];
    int i = 18;
    buf[i--] = '\0';
    long numCopy = x;
    while(numCopy) {
        int remain = numCopy % 16;
        if (remain < 10) {
            buf[i--] = 48 + remain;
        }
        else {
            buf[i--] = 65 + (remain - 10);
        }
        numCopy = numCopy >> 4;
    }
    buf[i--] = 'x';
    buf[i] = '0';
    printString((char*)(buf) + i);
}


void printVoid(void* v) {
    printHex((long)v);
}
