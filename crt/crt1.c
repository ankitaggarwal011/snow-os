#include <stdlib.h>

void _start(void) {
    // call main() and exit() here
    char *args[] = {"bin/name"};
    main(1, args, NULL);
}
