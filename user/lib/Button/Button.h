#ifndef BUTTON_H
#define BUTTON_H

#include "stdint.h"

typedef enum {
    BUTTON_STATE_NOT_PRESSED,
    BUTTON_STATE_SHORT_PRESS,
    BUTTON_STATE_SHORT_CLICK,
    BUTTON_STATE_LONG_CLICK,
} Button_State_t;

typedef enum {
    BUTTON_NO_TRANSITION,
    BUTTON_PRESSED,
    BUTTON_RELEASED,
} Button_Transition_t;

#ifdef __cplusplus
extern "C" {
#endif

void Button_Init();
void Button_Task();
uint8_t Button_Get(void);
Button_State_t Button_StateGet(void);
uint8_t * Button_GetRawValues(void);
Button_Transition_t Button_GetTransition();

#ifdef __cplusplus
}
#endif

#endif /* BUTTON_H */
