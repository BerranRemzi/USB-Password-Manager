#include "Menu.h"
#include <stdbool.h>
#include "Button.h"
#include "Serial.h"
#include "Password.h"
#include "main.h"
#include "SerialMenu.h"
#define SEC_TO_TICK(SEC) (SEC*100u)
#define PASSWORD_ENTER_TIMEOUT SEC_TO_TICK(5u)
#define DEVICE_LOCK_TIMEOUT SEC_TO_TICK(60u)

const char pleaseEnterPin[] = "\nPlease enter PIN: ";
const char pinCorrect[] = "\nPIN correct\n";
const char deviceLocked[] = "\nDevice locked\n";

typedef enum {
    MENU_INACTIVE,
    MENU_REQUEST_PIN_TEXT,
    MENU_WAIT_FOR_PIN,
    MENU_PIN_CORRECT_TEXT,
    MENU_PIN_CORRECT,
    MENU_SWITCH_MODE,
    MENU_INVALID
} Menu_Mode_t;

static Menu_Mode_t mode = MENU_REQUEST_PIN_TEXT;
uint32_t timeout = 0;
uint32_t menuDelay = 0u;
char Menu_buffer[128] = {'\0'};

void Menu_Init(void) {
    SerialMenu(Menu_buffer);
}

extern unsigned char _rx_buffer[16];


void Menu_Task(void) {
    uint8_t button = Button_Get();
    Button_State_t state = Button_StateGet();

    if (timeout > 0) {
        timeout--;
    }
    if (timeout == 1u) {
        mode = MENU_REQUEST_PIN_TEXT;
    }

    switch (mode) {
#if 0
    case MENU_INACTIVE:
        Password_Init();
        if (Button_StateGet() == BUTTON_STATE_LONG_PRESSED) {
            if (1u == button) {
                mode = MENU_REQUEST_PIN_TEXT;
            } else if (6u == button) {
                Bootloader_Activate();
            }
        }
        break;
#endif
    case MENU_REQUEST_PIN_TEXT:
        Password_Init();
        Serial_print(pleaseEnterPin);
        mode = MENU_WAIT_FOR_PIN;
        break;

    case MENU_WAIT_FOR_PIN:
        if ((state == BUTTON_STATE_SHORT_PRESS) && (button > 0u)) {
            timeout = PASSWORD_ENTER_TIMEOUT;
            Serial_write((uint8_t) '0' + button);
            Password_Put((uint8_t) '0' + button);

        }
        if (Password_Check() > 0u) {
            mode = MENU_PIN_CORRECT_TEXT;
            Serial_print(pinCorrect);
        }
        break;

    case MENU_PIN_CORRECT_TEXT:
        if (Button_GetTransition() == BUTTON_RELEASED) {
            mode = MENU_PIN_CORRECT;
        }
        break;

    case MENU_PIN_CORRECT:
        if (button > 0u) {
            timeout = DEVICE_LOCK_TIMEOUT;

            switch (state) {
            case BUTTON_STATE_SHORT_PRESS:
                //Serial_print("BUTTON_STATE_SHORT_PRESS\n");
                break;
            case BUTTON_STATE_LONG_PRESS:
                Serial_print("Switch mode\n");
                mode = MENU_SWITCH_MODE;
                break;
            case BUTTON_STATE_SHORT_CLICK:
                Serial_print("Slot A\n");
                break;
            case BUTTON_STATE_LONG_CLICK:
                Serial_print("Slot B\n");
                break;
            default:
                break;
            }
        }
        break;

    case MENU_SWITCH_MODE:
        break;

    default:
        mode = MENU_REQUEST_PIN_TEXT;
        break;
    }

    if (Serial_isDataReady(Menu_buffer) == true)
    {
      SerialMenu(Menu_buffer);
    }
}
