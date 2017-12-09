#ifndef STRING_H
#define STRING_H

//typedef unsigned long size_t;

#include <sys/defs.h>

char *strcat(char *s1, const char *s2);

int kstrcmp(const char *s1, const char *s2);

int32_t o_to_d(int n);

int32_t atoi(char *p);

uint64_t pow(int a, int b);

size_t strlen(const char *s);

void itoa(char *buf, int64_t val);

const char *strcpy(char *dest, const char *src);

#endif
