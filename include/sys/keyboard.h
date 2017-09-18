//
// Created by Saraj Munjal on 9/18/17.
//

#ifndef SNOW_OS_KEYBOARD_H
#define SNOW_OS_KEYBOARD_H
#define KB_CNTRL_ADDRESS 0x60
void keyboard_intr_handler();
void init_keyboard();
#endif //SNOW_OS_KEYBOARD_H
