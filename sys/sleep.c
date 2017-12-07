#include <sys/defs.h>

volatile int sleep_val = 0;

void on_second_tick() {
    if (sleep_val == 0) {
        return;
    }
    sleep_val--;
}

unsigned int sleep(unsigned int seconds) {
    sleep_val = seconds;
    while (1) {
        if (sleep_val == 0) {
            break;
        }
    }
    return 0;
}