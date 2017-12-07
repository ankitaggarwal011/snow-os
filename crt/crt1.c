#include <stdlib.h>

void _start(void) {
    // call main() and exit() here
    char *args[] = {"binname", "etc/test.txt"};
    main(2, args, NULL);
}
