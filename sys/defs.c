#include <sys/defs.h>

void* memset(void* a, int v, size_t b) {
    unsigned char* p = a;
    while(b-- > 0) *p++ = (unsigned char) v;
    return a;
}