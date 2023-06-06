#define  _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "SerialMenu.h"
#include "Serial.h"
//#include "main.h"
#include "Eeprom.h"

#if 0
Slot_t slot[4] = {
    {
        "Test",
        {"url"},
        {"userName"},
        {"password"}
    }
};
#endif

char* input;
char buffer[64];

extern const SerialMenuEntry menu_main[]; //__attribute__((section(".rodata")));
extern const SerialMenuEntry menu_slot[]; //__attribute__((section(".rodata")));
extern const SerialMenuEntry menu_slot_label_change[]; //__attribute__((section(".rodata")));
extern const SerialMenuEntry menu_slot_field[]; //__attribute__((section(".rodata")));
extern const SerialMenuEntry menu_slot_field_data_change[]; //__attribute__((section(".rodata")));
extern const SerialMenuEntry menu_slot_field_delay_change[]; //__attribute__((section(".rodata")));
extern const SerialMenuEntry menu_slot_field_action_change[]; //__attribute__((section(".rodata")));

const SerialMenuEntry* pMenuEntry = menu_main;
Slot_t* pSlot = NULL;
Field_t* pField = NULL;

static const char* text_EnterNewValue = "Enter new value: ";
//static const char* text_Empty = "Empty";

//char pLabel[12][16];

const SerialMenuEntry menu_main[] = {
    { "0", "Save", menu_main, []() { Eeprom_WriteChanged(); } },
    { "1a", label[0], menu_slot,  []() { pSlot = (Slot_t*)Eeprom_GetDataPtr(0); } },
    { "1b", label[1], menu_slot,  []() { pSlot = (Slot_t*)Eeprom_GetDataPtr(1); } },
    { "2a", label[2], menu_slot,  []() { pSlot = (Slot_t*)Eeprom_GetDataPtr(2); } },
    { "2b", label[3], menu_slot,  []() { pSlot = (Slot_t*)Eeprom_GetDataPtr(3); } },
    { "3a", label[4], menu_slot,  []() { pSlot = (Slot_t*)Eeprom_GetDataPtr(4); } },
    { "3b", label[5], menu_slot,  []() { pSlot = (Slot_t*)Eeprom_GetDataPtr(5); } },
    { "4a", label[6], menu_slot,  []() { pSlot = (Slot_t*)Eeprom_GetDataPtr(6); } },
    { "4b", label[7], menu_slot,  []() { pSlot = (Slot_t*)Eeprom_GetDataPtr(7); } },
    { "5a", label[8], menu_slot,  []() { pSlot = (Slot_t*)Eeprom_GetDataPtr(8); } },
    { "5b", label[9], menu_slot,  []() { pSlot = (Slot_t*)Eeprom_GetDataPtr(9); } },
    { "6a", label[10], menu_slot, []() { pSlot = (Slot_t*)Eeprom_GetDataPtr(10); } },
    { "6b", label[11], menu_slot, []() { pSlot = (Slot_t*)Eeprom_GetDataPtr(11); } },
    { NULL },
};

const SerialMenuEntry menu_slot[] = {
    { "0", "Back     ", menu_main, []() {} },
    { "1", "Label    ", menu_slot_label_change, []() {}, []() { Serial_print(pSlot->label); } },
    { "2", "URL      ", menu_slot_field, []() { pField = &pSlot->url; }, []() { Serial_print(pSlot->url.data); } },
    { "3", "Username ", menu_slot_field, []() { pField = &pSlot->userName; }, []() { Serial_print(pSlot->userName.data); } },
    { "4", "Password ", menu_slot_field, []() { pField = &pSlot->password; } },
    { NULL }
};

const SerialMenuEntry menu_slot_field[] = {
    { "0", "Back    ", menu_slot, []() {} },
    { "1", "Data    ", menu_slot_field_data_change, []() {}, []() { Serial_print(pField->data); } },
    { "2", "Delay   ", menu_slot_field_delay_change, []() {}, []() { Serial_write(pField->delay); } },
    { "3", "Action  ", menu_slot_field_action_change, []() {}, []() { Serial_write(pField->action); } },
    { NULL },
};

const SerialMenuEntry menu_slot_label_change[] = {
    { "", text_EnterNewValue, menu_slot, []() { memcpy(pSlot->label, input, sizeof(pSlot->label)); } },
    { NULL }
};

const SerialMenuEntry menu_slot_field_data_change[] = {
    { "", text_EnterNewValue, menu_slot_field, []() { memcpy(pField->data, input, sizeof(pField->data)); } },
    { NULL }
};

const SerialMenuEntry menu_slot_field_delay_change[] = {
    { "", text_EnterNewValue, menu_slot_field, []() { pField->delay = input[0]; } },
    { NULL }
};

const SerialMenuEntry menu_slot_field_action_change[] = {
    {"", text_EnterNewValue, menu_slot_field, []() { pField->action = input[0]; } },
    { NULL }
};

void SerialMenu(char* serialinput) {
    input = serialinput;

    // compare input string
    for (int i = 0; NULL != pMenuEntry[i].key; i++)
    {
        if ('\0' == pMenuEntry[i].key[0] || 0 == strcmp(pMenuEntry[i].key, serialinput))
        {
            pMenuEntry[i].runWhenSelected();
            pMenuEntry = (SerialMenuEntry*)pMenuEntry[i].pSerialMenuEntry;
            break;
        }
    }

    // Print menu
    for (int i = 0; NULL != pMenuEntry[i].key; i++)
    {
        if (pMenuEntry[i].key[0] == '\0')
        {
            sprintf(buffer, "%s", pMenuEntry[i].caption);
        }
        else if((pMenuEntry[i].caption[0] == 0) || (pMenuEntry[i].caption[0] == 255)) {
            sprintf(buffer, "%4s - Empty", pMenuEntry[i].key);
        } else {
            sprintf(buffer, "%4s - %.16s", pMenuEntry[i].key, pMenuEntry[i].caption);
        }
        Serial_print(buffer);
        if (NULL != pMenuEntry[i].runAlways)
        {
            pMenuEntry[i].runAlways();
        }
        if (pMenuEntry[i].key[0] != '\0')
        {
            Serial_print("\n");
        }
    }
    if (pMenuEntry->key != NULL && pMenuEntry->key[0] != '\0')
    {
        Serial_print("Enter your choice: ");
    }
}

#if 0
int main(void) {

    pMenuEntry = menu_main;
    char input[64];
    SerialMenu(input);
    while (1) {
        SerialMenu(input);
        scanf("%56s", input);
        //memcpy(label[0], input, 16);
    }
    return 0;
}
#else

#endif
