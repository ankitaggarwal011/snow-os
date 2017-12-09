#include <stdlib.h>

void _start(int argc, char *argv[]) {
    // call main() and exit() here
    int exit_code = main(argc, argv, NULL);
    exit(exit_code);
}
