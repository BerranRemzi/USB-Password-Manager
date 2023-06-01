#ifndef BUTTON_H
#define BUTTON_H

#include "stdint.h"

typedef enum {
    BUTTON_STATE_NOT_PRESSED,
    BUTTON_STATE_PRESSED,
    BUTTON_STATE_SHORT_PRESSED,
    BUTTON_STATE_LONG_PRESSED,
} Button_t;

#ifdef __cplusplus
extern "C" {
#endif

void Button_Init();
void Button_Task();
uint8_t Button_Get(void);
Button_t Button_StateGet(void);
uint8_t * Button_GetRawValues(void);

#ifdef __cplusplus
}
#endif

#endif /* BUTTON_H */
