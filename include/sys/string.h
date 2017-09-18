#ifndef STRING_H
#define STRING_H
//typedef unsigned long size_t;
#define size_t __builtin_size_t
#define memcpy __builtin_memcpy
char* strcat(char *s1, const char *s2);
#endif
