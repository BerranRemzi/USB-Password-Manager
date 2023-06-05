#ifndef EEPROM_H
#define EEPROM_H
#include "stdint.h"

typedef struct {
	char data[56];
	uint8_t delay;
	uint8_t action;
} Field_t;

typedef struct {
	char label[16];
	Field_t url;
	Field_t userName;
	Field_t password;
} Slot_t;

Slot_t* Eeprom_GetDataPtr(uint8_t index);
void Eeprom_Save();
extern const char* label[12];
#endif /* EEPROM_H */