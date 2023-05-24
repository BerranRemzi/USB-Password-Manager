#ifndef KEYBOARD_H
#define KEYBOARD_H
#include "stdint.h"

typedef struct
{
    uint8_t MODIFIER;
    uint8_t RESERVED;
    uint8_t KEYCODE1;
    uint8_t KEYCODE2;
    uint8_t KEYCODE3;
    uint8_t KEYCODE4;
    uint8_t KEYCODE5;
    uint8_t KEYCODE6;
} Keyboard_HID_t;


void Keyboard_Print(uint8_t * data);
void Keyboard_Write(uint8_t data);

#endif /* KEYBOARD_H */
