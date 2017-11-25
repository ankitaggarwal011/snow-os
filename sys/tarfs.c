#include <sys/defs.h>
#include <sys/kprintf.h>
#include <sys/physical_memory.h>
#include <sys/string.h>
#include <sys/tarfs.h>

struct posix_header_ustar *tarfs_start = (struct posix_header_ustar*) &_binary_tarfs_start;
struct posix_header_ustar *tarfs_end = (struct posix_header_ustar*) &_binary_tarfs_end;

int atoi(char *str) {
    int sign = 1;
    if (*(str) == '-') {
        sign = -1;
    }
    str++;
    int ans = 0;
    while (*str) {
        if ((int)(*str) >= 48 && (int)(*str) <= 57) {
            ans = (ans << 3) + (ans << 1) + (*str) - 48;
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

void print_all_files() {
    struct posix_header_ustar *s = tarfs_start;
    do {
        int file_size = o_to_d(atoi(s->size));
        
        kprintf("Name: %s, Size: %d bytes, Type: %s\n", s->name, file_size, s->typeflag);

        if (file_size > 0) {
            s += (file_size / (sizeof(s) + 1)) + 2;
        } else {
            s++;
        }
    } while(s < tarfs_end);
}

void* get_file(char *filename) {
    struct posix_header_ustar *s = tarfs_start;
    do {
        int file_size = o_to_d(atoi(s->size));
        char *file = (char*) (s + 1);
        
        if (kstrcmp(filename, s->name) == 0) {
            kprintf("File found: Name: %s, Size: %d bytes, Type: %s\n", s->name, file_size, s->typeflag);  
            return (void*) file;
        }
       
        if (file_size > 0) {
            s += (file_size / (sizeof(s) + 1)) + 2;
        } else {
            s++;
        }
    } while(s < tarfs_end);
    
    kprintf("File not found.\n");
    return NULL;
 }

void init_tarfs() {
    print_all_files();
    // get_file(char *filename);
}