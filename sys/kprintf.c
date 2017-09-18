#include <sys/kprintf.h>
#include <sys/stdarg.h>
#include <sys/string.h>

long currentRow = 0;
long currentColumn = 0;
va_list args;

char *getAddress(long row, long column) {
    return (char *) (VIDEO_BASE_ADDRESS + 2 * (VIDEO_MEM_COLUMNS * row + column));
}

void kprintf(const char *fmt, ...) {
    va_start(args, fmt);
    const char *c = fmt;
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
        printString(va_arg(args,
        char*));
    } else if (ft == INT) {
        printLong(va_arg(args,
        int));
    } else if (ft == HEX || ft == VOID) {
        printHex(va_arg(args,
        long));
    }
}

void resetVideoMemory(char initValue, char color) {
    for (int i = 0; i < VIDEO_MEM_ROWS; i++) {
        for (int j = 0; j < VIDEO_MEM_COLUMNS; j++) {
            char *address = getAddress(i, j);
            *address = initValue;
            *(address + 1) = color;
        }
    }
}

void printChar(char c) {
    if (c == '\n') {
        if (currentRow == VIDEO_MEM_ROWS - 1) {
            currentRow = 0;
            memcpy(getAddress(VIDEO_MEM_ROWS, VIDEO_MEM_COLUMNS), getAddress(0, 0),
                   2 * VIDEO_MEM_ROWS * VIDEO_MEM_COLUMNS);
            resetVideoMemory(' ', DEFAULT_COLOR);


        } else {
            currentRow++;
        }
        currentColumn = 0;
        return;
    }
    char *address = getAddress(currentRow, currentColumn);
    *address = c;
    if (currentColumn == VIDEO_MEM_COLUMNS - 1) {
        if (currentRow == VIDEO_MEM_ROWS - 1) {
            currentRow = 0;
            memcpy(getAddress(VIDEO_MEM_ROWS, VIDEO_MEM_COLUMNS), getAddress(0, 0),
                   2 * VIDEO_MEM_ROWS * VIDEO_MEM_COLUMNS);
            resetVideoMemory(' ', DEFAULT_COLOR);
        } else {
            currentRow++;
        }
        currentColumn = 0;
    } else {
        currentColumn++;
    }
}

void printString(char *c) {
    while (*c != '\0') {
        printChar(*(c++));
    }
}

long mod(long x) {
    return x > 0 ? x : -x;
}

void printLong(long x) {
    char buf[66];
    int i = 65;
    buf[i--] = '\0';
    long numCopy = mod(x);
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

void printHex(long x) {
    char buf[19];
    int i = 18;
    buf[i--] = '\0';
    long numCopy = mod(x);
    do {
        int remain = numCopy % 16;
        if (remain < 10) {
            buf[i--] = 48 + remain;
        } else {
            buf[i--] = 65 + (remain - 10);
        }
        numCopy = numCopy >> 4;
    } while (numCopy);
    buf[i--] = 'x';
    buf[i] = '0';
    printString(buf + i);
}

void printTime(long x) {
    x /= 100;
    char *time_address = getAddress(VIDEO_MEM_ROWS, VIDEO_MEM_COLUMNS - 40);
    for (int j = 0; j < 80; j += 2) {
        *(time_address + j) = ' ';
        *(time_address + j + 1) = 7;
    }
    int i = 0, hh = x / 3600, mm = (x / 60) % 60, ss = x % 60;
    char buf[24];
    buf[i--] = '\0';
    buf[i--] = 's';
    do {
        buf[i--] = 48 + ss % 10;
        ss /= 10;
    } while (ss);
    buf[i--] = ' ';
    buf[i--] = ':';
    buf[i--] = ' ';
    buf[i--] = 'm';
    do {
        buf[i--] = 48 + mm % 10;
        mm /= 10;
    } while (mm);
    buf[i--] = ' ';
    buf[i--] = ':';
    buf[i--] = ' ';
    buf[i--] = 'h';
    do {
        buf[i--] = 48 + hh % 10;
        hh /= 10;
    } while (hh);
    i++;
    char *c = buf + i;
    char *tsb = "Time since boot: ";
    while (*tsb != '\0') {
        *time_address = *(tsb++);
        time_address += 2;
    }
    while (*c != '\0') {
        *time_address = *(c++);
        time_address += 2;
    }
}

void printInputChar(const char *c) {
    char *address = getAddress(VIDEO_MEM_ROWS, 0);
    for (int j = 0; j < 30; j += 2) {
        *(address + j) = ' ';
        *(address + j + 1) = 2;
    }
    char buf[15] = "Input: ";
    char *str = strcat(buf, c);
    int i = 0;
    while (str[i] != '\0') {
        *(address + 2 * i) = str[i];
        i++;
    }
}