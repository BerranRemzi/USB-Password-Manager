#include "Password.h"

#define PASSWORD_SIZE 16u
static char Password_buffer[PASSWORD_SIZE];

uint32_t oat_hash(const char *s, uint8_t len);

const uint32_t Password_hash[2] = {
        0x5CD2472Cu,
        0xEEF833C1u
};

uint8_t Password_pos = 0u;
void Password_Init(void) {
    Password_pos = 0u;
}

void Password_Put(char input) {
    Password_buffer[Password_pos] = input;

    if (Password_pos < PASSWORD_SIZE) {
        Password_pos++;
    }
}

uint8_t Password_Check(void) {
    uint8_t returnValue = 0u;
    uint32_t hash = oat_hash(Password_buffer, Password_pos);

    if (Password_hash[0] == hash) {
        returnValue = 1u;
        Password_pos = 0u;
    } else if (Password_hash[1] == hash) {
        returnValue = 2u;
        Password_pos = 0u;
    }

    return returnValue;
}

/*
 * Bob Jenkins "One-at-a-time" hash
 */
uint32_t oat_hash(const char *s, uint8_t len)
{
    unsigned char *p = (unsigned char*) s;
    uint32_t h = 0;

    while (len--) {
        h += *p++;
        h += (h << 10);
        h ^= (h >> 6);
    }

    h += (h << 3);
    h ^= (h >> 11);
    h += (h << 15);

    return h;
}
