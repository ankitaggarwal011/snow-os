#ifndef __KPRINTF_H
#define __KPRINTF_H

#define VIDEO_BASE_ADDRESS 0xb8000
#define VIDEO_MEM_ROWS 24
#define VIDEO_MEM_COLUMNS 80
#define DEFAULT_COLOR 7 //white

typedef enum format_type {
    STRING,
    INT,
    HEX,
    VOID
} format_type;

void printSpecial(int argNumber, format_type ft);
void printChar(char c);
void printString(char *c);
void printHex(long x);
void printLong(long x);
void printTime(long x);
void kprintf(const char *fmt, ...);
void printInputChar(const char *c);
#endif
