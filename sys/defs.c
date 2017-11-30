#include <sys/defs.h>

void* memset(void* a, int v, size_t b) {
    unsigned char *p = a;
    while(b-- > 0) *p++ = (unsigned char) v;
    return a;
}

void* memset_byte(void* a, uint64_t v, uint64_t b) {
    uint64_t *p = a;
    while(b-- > 0) *p++ = (uint64_t) v;
    return a;
}

void* memcpy(void *dest, void *src, int bytes) {
    char *ts = src, *td = dest;
    while (bytes--)
        *td++ = *ts++;
    return dest;
}