#ifndef KEYPAD_H
#define KEYPAD_H
#include "stdint.h"

uint8_t Keypad_GetKey(void);
void Keypad_Load(uint16_t _newSample);
bool Keypad_IsKeyDown(void);

#endif /* KEYPAD_H */