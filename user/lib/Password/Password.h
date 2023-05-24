#ifndef PASSWORD_H
#define PASSWORD_H
#include "stdint.h"

void Password_Init(void);
void Password_Put(char input);
uint8_t Password_Check(void);

#endif /* PASSWORD_H */
