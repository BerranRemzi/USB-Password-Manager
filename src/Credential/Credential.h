#ifndef PASSWORD_H
#define PASSWORD_H
#include "stdint.h"

typedef struct
{
    const char *const site;
    const char *const name;
    const char *const password;
} Credential_t;

void Credential_Task(void);
const char *Credential_GetSite(uint8_t number);
const char *Credential_GetName(uint8_t number);
const char *Credential_GetPassword(uint8_t number);
bool Credential_IsLogged(void);
void Credential_PutKey(uint8_t key);

#endif /* PASSWORD_H */