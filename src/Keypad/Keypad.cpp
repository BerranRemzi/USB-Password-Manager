#include "Keypad.h"
#include "Arduino.h"

#define KEYPAD_KEY_COUNT 8u

typedef struct
{
    uint8_t key;
    int16_t adc;
} Keypad_t;

const Keypad_t button[KEYPAD_KEY_COUNT]{
    {0, 1023},
    {1, 877},
    {2, 731},
    {3, 585},
    {4, 438},
    {5, 292},
    {6, 146},
    {0, 0},
};

static uint8_t Keypad_key = '\0';

uint8_t Keypad_GetKey(void)
{
    return Keypad_key;
}

void Keypad_Load(uint16_t _newSample)
{
    int16_t lastDiff = 0;
    for (uint8_t p = 0; p < KEYPAD_KEY_COUNT; p++)
    {
        int16_t newDiff = abs(_newSample - button[p].adc);
        if ((lastDiff >= newDiff) || (p == 0))
        {
            lastDiff = newDiff;
            Keypad_key = button[p].key;
        }
    }
}

bool Keypad_IsKeyDown(void){
    return false;
}