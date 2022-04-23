#include "Credential.h"
#include "Credential_cfg.h"
#include <avr/pgmspace.h>

#define CREDENTIAL_TIMEOUT_COUNT 10000
#define PIN_STORAGE_SIZE 6
#define CREDENTIAL_TABLE_SIZE 6

// Then set up a table to refer to your strings.
const PROGMEM Credential_t table[CREDENTIAL_TABLE_SIZE] = {
    {.site = site_1, .name = name_1, .password = password_1},
    {.site = site_2, .name = name_2, .password = password_2},
    {.site = site_3, .name = name_3, .password = password_3},
    {.site = site_4, .name = name_4, .password = password_4},
    {.site = site_5, .name = name_5, .password = password_5},
    {.site = site_6, .name = name_6, .password = password_6},
};

static const uint8_t pin[PIN_STORAGE_SIZE] = {1, 2, 3, 4, 5, 6};
static uint8_t pinStorage[PIN_STORAGE_SIZE];

static char buffer[64]; // make sure this is large enough for the largest string it must hold

static uint16_t timeoutCounter;
static bool isLogged = false;

void Credential_Task(void)
{
    if (timeoutCounter > 0)
    {
        timeoutCounter--;
    }
    else
    {
        isLogged = false;
    }
}

const char *Credential_GetSite(uint8_t number)
{
    strcpy_P(buffer, (char *)pgm_read_word(&(table[number - 1].site)));
    return (buffer);
}

const char *Credential_GetName(uint8_t number)
{
    strcpy_P(buffer, (char *)pgm_read_word(&(table[number - 1].name)));
    return (buffer);
}

const char *Credential_GetPassword(uint8_t number)
{
    strcpy_P(buffer, (char *)pgm_read_word(&(table[number - 1].password)));
    return (buffer);
}

bool Credential_IsLogged(void)
{
    return true;
}

void Credential_PutKey(uint8_t key)
{
    memcpy_P(&pinStorage[0], &pinStorage[1], PIN_STORAGE_SIZE);
    pinStorage[0] = key;
    if (0 == memcmp_P(pin, pinStorage, PIN_STORAGE_SIZE))
    {
        timeoutCounter = CREDENTIAL_TIMEOUT_COUNT;
        isLogged = true;
    }
}