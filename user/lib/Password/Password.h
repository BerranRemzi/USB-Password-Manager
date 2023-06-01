#ifndef PASSWORD_H
#define PASSWORD_H
#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

void Password_Init(void);
void Password_Put(char input);
uint8_t Password_Check(void);

#ifdef __cplusplus
}
#endif

#endif /* PASSWORD_H */
