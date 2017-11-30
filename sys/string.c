#include <sys/string.h>

char* strcat(char *s1, const char *s2) {
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
    return s1;
}

int kstrcmp(char *s1, char *s2) {
    while (*s1 && *s2 && *s1 == *s2) {
        s1++;
        s2++;
    }
    if (*s1 == 0 && *s2 == 0) {
        return 0;
    }
    else if (*s1 > *s2) {
        return 1;
    }
    return -1;
}

int atoi(char *str) {
    int sign = 1;
    if (*(str) == '-') {
        sign = -1;
    }
    str++;
    int ans = 0;
    while (*str) {
        if ((int)(*str) >= 48 && (int)(*str) <= 57) {
            ans = ans * 10 + (*str) - 48;
            str++;
        }
        else {
            return 0;
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
