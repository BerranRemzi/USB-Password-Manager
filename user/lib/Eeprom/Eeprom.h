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

extern const char* label[12];

#ifdef __cplusplus
extern "C" {
#endif

Slot_t* Eeprom_GetDataPtr(uint8_t index);
void Eeprom_WriteChanged();
void Eeprom_ReadPage(uint8_t block);
void Eeprom_ErasePage(uint8_t block);
void Eeprom_WritePage(uint8_t block);

#ifdef __cplusplus
}
#endif

#endif /* EEPROM_H */
