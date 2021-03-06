#include <sys/defs.h>

char *strcat(char *s1, const char *s2) {
    char *s = s1;
    while (*s != '\0') {
        s++;
    }
    const char *t = s2;
    while (*t != '\0') {
        *s = *t;
        s++;
        t++;
    }
    *s = '\0';
    return s1;
}

int strcmp(const char *s1, const char *s2) {
    while (*s1 && *s2 && *s1 == *s2) {
        s1++;
        s2++;
    }
    return *s1 - *s2;
}

void itoa_cl(char *buf, int64_t val) {
    if (val == 0) {
        buf[0] = '0';
        buf[1] = '\0';
        return;
    }
    uint64_t bkup = val >= 0 ? val : -val;
    int n_digits = 0;
    while (bkup) {
        bkup /= 10;
        n_digits++;
    }

    int i = n_digits;
    if (val < 0) {
        i++;
    }
    buf[i--] = '\0';
    uint64_t numCopy = val >= 0 ? val : -val;
    do {
        buf[i--] = 48 + numCopy % 10;
        numCopy /= 10;
    } while (numCopy);
    if (val < 0) {
        buf[i] = '-';
    } else {
        i++;
    }
    return;
}

int atoi(char *str) {
    int sign = 1;
    if (*(str) == '-') {
        sign = -1;
        str++;
    } else if (*(str) == '+') {
        sign = 1;
        str++;
    }
    int ans = 0;
    while (*str) {
        if ((int) (*str) >= 48 && (int) (*str) <= 57) {
            ans = ans * 10 + (*str) - 48;
            str++;
        } else {
            return -1;
        }
    }
    return ans * sign;
}

uint64_t pow(int a, int b) {
    uint64_t ans = 1;
    while (b--) {
        ans *= a;
    }
    return ans;
}

int o_to_d(int n) {
    int i = 0, r, ans = 0;
    while (n > 0) {
        r = n % 10;
        n /= 10;
        ans += (r * pow(8, i));
        i++;
    }
    return ans;
}


size_t strlen(const char *s) {
    if (s == NULL || '\0' == *s) {
        return 0;
    }
    size_t len = 0;
    while ((*s) != '\0') {
        s++;
        len++;
    }
    return len;
}

const char *strcpy(char *dest, const char *src) {
    const char *str = dest;
    while(*src) {
        *dest = *src;
        src++;
        dest++;
    }
    *dest = '\0';
    return str;
}

void *memcpy(void *dest, void *src, int bytes) {
    const unsigned char *ts = src;
    unsigned char *td = dest;
    for (int i = 0; i < bytes; i++) {
        td[i] = ts[i];
    }
    return dest;
}

void *memset(void *a, int v, size_t b) {
    unsigned char *p = a;
    for (int i = 0; i < b; i++) {
        p[i] = (unsigned char) v;
    }
    return a;
}