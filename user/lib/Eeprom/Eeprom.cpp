#include "Eeprom.h"
#include "stdint.h"
#include "string.h"

#define BLOCK_SIZE 1024u
uint8_t eeprom[3u][BLOCK_SIZE];
uint8_t ram[BLOCK_SIZE];
uint8_t currentBlock = 0;
uint8_t prevBlock = 0xFF;

const char* label[12] = {
    (char*)&eeprom[0][0 * sizeof(Slot_t)],
    (char*)&eeprom[0][1 * sizeof(Slot_t)],
    (char*)&eeprom[0][2 * sizeof(Slot_t)],
    (char*)&eeprom[0][3 * sizeof(Slot_t)],
    (char*)&eeprom[1][0 * sizeof(Slot_t)],
    (char*)&eeprom[1][1 * sizeof(Slot_t)],
    (char*)&eeprom[1][2 * sizeof(Slot_t)],
    (char*)&eeprom[1][3 * sizeof(Slot_t)],
    (char*)&eeprom[2][0 * sizeof(Slot_t)],
    (char*)&eeprom[2][1 * sizeof(Slot_t)],
    (char*)&eeprom[2][2 * sizeof(Slot_t)],
    (char*)&eeprom[2][3 * sizeof(Slot_t)],
};

void Eeprom_Init() {

}

Slot_t* Eeprom_GetDataPtr(uint8_t index) {
    currentBlock = index / 4u;

    if (prevBlock == 0xFF) {
        (void)memcpy((uint8_t*)ram, (uint8_t*)&eeprom[currentBlock], sizeof(ram));
    }
    else if (prevBlock != currentBlock) {
        (void)memcpy((uint8_t*)&eeprom[prevBlock], (uint8_t*)ram, sizeof(ram));
        (void)memcpy((uint8_t*)ram, (uint8_t*)&eeprom[currentBlock], sizeof(ram));
    }
    prevBlock = currentBlock;

    uint32_t address = (index % 4u) * sizeof(Slot_t);
    return (Slot_t*)&ram[address];
}

void Eeprom_Save() {
    (void)memcpy((uint8_t*)&eeprom[prevBlock], (uint8_t*)ram, sizeof(ram));
}