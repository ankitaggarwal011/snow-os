#include <sys/defs.h>

void *memset(void *a, int v, size_t b) {
    unsigned char *p = a;
    for (int i = 0; i < b; i++) {
        p[i] = (unsigned char) v;
    }
    return a;
}

void *memset_byte(void *a, uint64_t v, uint64_t b) {
    uint64_t *p = a;
    for (int i = 0; i < b; i++) {
        p[i] = (uint64_t) v;
    }
    return a;
}

void *memcpy(void *dest, void *src, int bytes) {
    const unsigned char *ts = src;
    unsigned char *td = dest;
    for (int i = 0; i < bytes; i++) {
        td[i] = ts[i];
    }
    return dest;
}