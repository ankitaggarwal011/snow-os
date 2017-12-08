#ifndef USER_SPACE_STRING_H
#define USER_SPACE_STRING_H

#include <sys/defs.h>

char *strcat(char *s1, const char *s2);

int strcmp(char *s1, char *s2);

int32_t o_to_d(int n);

int32_t atoi(char *p);

uint64_t pow(int a, int b);

size_t strlen(char *s);

void itoa_cl(char *buf, int64_t val);

char *strcpy(char *dest, const char *src);

void *memcpy(void *dest, void *src, int bytes);

#endif
