#ifndef CREDENTIAL_CFG_H
#define CREDENTIAL_CFG_H

#include "Credential.h"
#include <avr/pgmspace.h>

#define SITE(NUMBER, SITE_STRING) const PROGMEM char site_##NUMBER[] = SITE_STRING
#define NAME(NUMBER, NAME_STRING) const PROGMEM char name_##NUMBER[] = NAME_STRING
#define PASSWORD(NUMBER, PASSWORD_STRING) const PROGMEM char password_##NUMBER[] = PASSWORD_STRING

#define CREDENTIAL(NUMBER, SITE_STRING, NAME_STRING, PASSWORD_STRING) \
    SITE(NUMBER, SITE_STRING);                                        \
    NAME(NUMBER, NAME_STRING);                                        \
    PASSWORD(NUMBER, PASSWORD_STRING);

#define EXTERN_CREDENTIAL(NUMBER)             \
    extern const PROGMEM char site_##NUMBER[]; \
    extern const PROGMEM char name_##NUMBER[]; \
    extern const PROGMEM char password_##NUMBER[];

EXTERN_CREDENTIAL(1);
EXTERN_CREDENTIAL(2);
EXTERN_CREDENTIAL(3);
EXTERN_CREDENTIAL(4);
EXTERN_CREDENTIAL(5);
EXTERN_CREDENTIAL(6);

#endif /* CREDENTIAL_CFG_H */