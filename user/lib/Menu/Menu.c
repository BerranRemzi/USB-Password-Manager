#include "Menu.h"
#include <stdbool.h>
#include "Button.h"
#include "Serial.h"
#include "Password.h"

#define PASSWORD_ENTER_TIMEOUT 500u

const uint8_t pleaseEnterPin[] = "\nPlease enter PIN: ";
const uint8_t pinCorrect[] = "\nPIN correct\n";
const uint8_t deviceLocked[] = "\nDevice locked\n";

typedef enum {
    MENU_INACTIVE,
    MENU_REQUEST_PIN,
    MENU_WAIT_FOR_PIN,
    MENU_PIN_CORRECT,
    MENU_INVALID
} Menu_Mode_t;

static Menu_Mode_t mode = MENU_INACTIVE;
uint16_t timeout = 0;

void Menu_Init(void) {

}

extern unsigned char _rx_buffer[16];

void Menu_Task(void) {
    uint8_t button = Button_Get();
    if (button > 0u) {
        timeout = PASSWORD_ENTER_TIMEOUT;
    }

    if (timeout > 0) {
        timeout--;
    } else {
        mode = MENU_INACTIVE;
    }

    if (timeout == 1u) {
        Serial_print(deviceLocked);
    }

    switch (mode) {
    case MENU_INACTIVE:
        Password_Init();
        if ((Button_StateGet() == BUTTON_STATE_LONG_PRESSED)
                && (1u == button)) {
            mode = MENU_REQUEST_PIN;
        }
        break;
    case MENU_REQUEST_PIN:
        Serial_print(pleaseEnterPin);
        mode = MENU_WAIT_FOR_PIN;
        break;
    case MENU_WAIT_FOR_PIN:
        if ((Button_StateGet() == BUTTON_STATE_PRESSED) && (button > 0u)) {
            Serial_write((uint8_t)'0' + button);
            Password_Put((uint8_t)'0' + button);
            if (Password_Check() > 0u) {
                mode = MENU_PIN_CORRECT;
            }
        }
        if (Serial_Available() > 0) {
            uint8_t read = Serial_Read();
            Serial_write(&read);
        }
        break;
    case MENU_PIN_CORRECT:
        Serial_print(pinCorrect);
        mode = MENU_INACTIVE;

        break;
    default:
        break;
    }
}
