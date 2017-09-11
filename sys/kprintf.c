#include <sys/kprintf.h>

char* outputWriteAddress = (char*)0xb8000;
long totalCharsPrinted = 0;
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

void kprintf(const char *fmt, ...)
{
    const char* c = &fmt[0];
    int specialCharCount = 0;
    while (*c!='\0') {
        if (*c == '%') {
            //special character case
            switch (*(c+1)) {
                case 's':
                    break;
                case 'd':
                    break;
                case 'x':
                    break;
                case 'p':
                    break;
                default:
                    break;
            }
            specialCharCount++;
            c+=2;
        } else {
            printChar(*c);
            c++;
        }
    }
    printString("Hello");
}

void printSpecial(int argNumber, format_type ft) {
    switch (argNumber) {
        case 0:
            //extract RDI;
            break;
        case 1:
//            extract RSI:
            break;
        case 2:
//            extract RDX;
            break;
        case 3:
//            extract RCX;
            break;
        case 4:
//            extract R8:
            break;
        case 5:
//            extract R9:
            break;

        default:
            break;
    }
}


void printChar(char c) {
    (*outputWriteAddress) = c;
    outputWriteAddress += 2;
    totalCharsPrinted++;
}


void printString(char *c) {
    while (c!='\0') {
        printChar(*(c++));
    }
}

void printLong(long x) {
    char buf[66];
    int i = 66;
    buf[i--] = '\0';
    long numCopy = x;
    while(numCopy) {
        buf[i--] = numCopy % 10;
        numCopy /= 10;
    }
    if (x < 0) {
        buf[i] = '-';
    }
    printString(buf + i);
}

void printHex(long x) {
    char buf[19];
    int i = 19;
    buf[i--] = '\0';
    long numCopy = x;
    while(numCopy) {
        buf[i--] = numCopy % 16;
        numCopy = numCopy >> 4;
    }
    buf[i--] = 'x';
    buf[i] = '0';
    printString(buf + i);
}


void printVoid(void* v) {
    //TODO printVoid
}