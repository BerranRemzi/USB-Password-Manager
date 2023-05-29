#define  _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "SerialMenu.h"

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

Slot_t slot[4] = {
    {
        "Test",
        {"url"},
        {"userName"},
        {"password"}
    }
};

char input[64];
const SerialMenuEntry *pMenuEntry;

extern const SerialMenuEntry menu_main[] __attribute__((section(".rodata")));
extern const SerialMenuEntry menu_slot[] __attribute__((section(".rodata")));
extern const SerialMenuEntry menu_slot_label_change[] __attribute__((section(".rodata")));
extern const SerialMenuEntry menu_slot_field[] __attribute__((section(".rodata")));
extern const SerialMenuEntry menu_slot_field_data_change[] __attribute__((section(".rodata")));
extern const SerialMenuEntry menu_slot_field_delay_change[] __attribute__((section(".rodata")));
extern const SerialMenuEntry menu_slot_field_action_change[] __attribute__((section(".rodata")));

Slot_t *pSlot;
Field_t *pField;

static const char * text_EnterNewValue = "Enter new value: ";

const SerialMenuEntry menu_main[14] = {
    { "0", "Save", menu_main, []() { exit(0); } },
    { "1a", slot[0].label, menu_slot, []() { pSlot = &slot[0]; } },
    { "1b", slot[1].label, menu_slot, []() { pSlot = &slot[1]; } },
    { "2a", slot[2].label, menu_slot, []() { pSlot = &slot[2]; } },
    { "2b", slot[3].label, menu_slot, []() { pSlot = &slot[3]; } },
    { NULL }
};

const SerialMenuEntry menu_slot[] = {
    { "0", "Back     ", menu_main, []() {} },
    { "1", "Label    ", menu_slot_label_change, []() {}, []() { printf(": %s", pSlot->label); } },
    { "2", "URL      ", menu_slot_field, []() { pField = &pSlot->url; }, []() { printf(": %s", pSlot->url.data); } },
    { "3", "Username ", menu_slot_field, []() { pField = &pSlot->userName; }, []() { printf(": %s", pSlot->userName.data); } },
    { "4", "Password ", menu_slot_field, []() { pField = &pSlot->password; } },
    { NULL }
};

const SerialMenuEntry menu_slot_field[] = {
    { "0", "Back    ", menu_slot, []() {} },
    { "1", "Data    ", menu_slot_field_data_change, []() {}, []() { printf(": %s", pField->data); } },
    { "2", "Delay   ", menu_slot_field_delay_change, []() {}, []() { printf(": %d", pField->delay); } },
    { "3", "Action  ", menu_slot_field_action_change, []() {}, []() { printf(": %c", pField->action); } },
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
    { text_EnterNewValue, "", menu_slot_field, []() { pField->delay = atoi(input); } },
    { NULL }
};

const SerialMenuEntry menu_slot_field_action_change[] = {
    {"", text_EnterNewValue, menu_slot_field, []() { pField->action = input[0]; } },
    { NULL }
};

void SerialMenu(char *input) {
	//compare input string
	for (int i = 0; NULL != pMenuEntry[i].key; i++) {
		if ('\0' == pMenuEntry[i].key[0]
				|| 0 == strcmp(pMenuEntry[i].key, input)) {
			pMenuEntry[i].runWhenSelected();
			pMenuEntry = (SerialMenuEntry*) pMenuEntry[i].pSerialMenuEntry;
			break;
		}
	}

	//Print menu
	for (int i = 0; NULL != pMenuEntry[i].key; i++) {
		if (pMenuEntry[i].key[0] == '\0') {
			printf("%s", pMenuEntry[i].caption);
		} else {
			printf("%4s - %s", pMenuEntry[i].key, pMenuEntry[i].caption);
		}
		if (NULL != pMenuEntry[i].runAlways) {
			pMenuEntry[i].runAlways();
		}
		if (pMenuEntry[i].key[0] != '\0') {
			printf("\n");
		}
	}
}

#if 0
void main(void) {
    pMenuEntry = menu_main;

    while (1) {
        SerialMenu(input);

        if (pMenuEntry->key[0] != '\0') {
            printf("Enter your choice: ");
        }
        else {
            printf("%s", customText);
            customText[0] = '\0';
        }
        scanf("%56s", input);
        //memcpy(label[0], input, 16);
    }
}
#else

#endif
