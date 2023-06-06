#include "Eeprom.h"
#include "string.h"
#include "stm32f0xx_hal.h"

#define ADDR_FLASH_PAGE_00    (0x08000000u) /* Base @ of Page 00, 1 Kbytes */
#define ADDR_FLASH_PAGE_29    (0x08007400u) /* Base @ of Page 00, 1 Kbytes */
#define ADDR_FLASH_PAGE_30    (0x08007800u) /* Base @ of Page 00, 1 Kbytes */
#define ADDR_FLASH_PAGE_31    (0x08007C00u) /* Base @ of Page 31, 1 Kbytes */

const uint8_t *const eeprom[3] = {
        (const uint8_t*) ADDR_FLASH_PAGE_29,
        (const uint8_t*) ADDR_FLASH_PAGE_30,
        (const uint8_t*) ADDR_FLASH_PAGE_31
};

uint8_t ram[FLASH_PAGE_SIZE];
uint8_t currentBlock = 0;
uint8_t prevBlock = 0xFF;

const char* label[12] =
{
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
        (void)memcpy((uint8_t*)ram, (uint8_t*)eeprom[currentBlock], sizeof(ram));
    }
    else if (prevBlock != currentBlock) {
        Eeprom_WritePage(prevBlock);
        Eeprom_ReadPage(currentBlock);
    }
    prevBlock = currentBlock;

    uint32_t address = (index % 4u) * sizeof(Slot_t);
    return (Slot_t*)&ram[address];
}

void Eeprom_WritePage(uint8_t page) {
#if 0
    (void)memcpy((uint8_t*)&eeprom[page], (uint8_t*)ram, sizeof(ram));
#else
    __disable_irq();
    Eeprom_ErasePage(page);

    (void) HAL_FLASH_Unlock();
    for (uint32_t i = 0; i < FLASH_PAGE_SIZE; i += 4u) {
        (void) HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, (uint32_t)(eeprom[page]+i), *((uint32_t*)(ram+i)));
    }
    (void) HAL_FLASH_Lock();
    __enable_irq();
#endif
}

void Eeprom_ReadPage(uint8_t page) {
    (void)memcpy((uint8_t*)ram, (uint8_t*)eeprom[page], sizeof(ram));
}

void Eeprom_ErasePage(uint8_t page) {
    (void) HAL_FLASH_Unlock();
    FLASH_EraseInitTypeDef eraseInitTypeDef =
    {
         .TypeErase = FLASH_TYPEERASE_PAGES,
         .PageAddress = (uint32_t)eeprom[page],
         .NbPages = 1
    };
    uint32_t pageError;
    HAL_FLASHEx_Erase(&eraseInitTypeDef, &pageError);
    (void) HAL_FLASH_Lock();
}

void Eeprom_WriteChanged() {
#if 0
    (void)memcpy((uint8_t*)&eeprom[prevBlock], (uint8_t*)ram, sizeof(ram));
#else
    Eeprom_WritePage(prevBlock);
#endif
}
