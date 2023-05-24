#include "Keyboard.h"

#include "scancode-ascii-table.h"
#include "usb_device.h"

#define KEYBOARD_DELAY 20u

Keyboard_HID_t Keyboard_hid = {
        0, 0, 0, 0, 0, 0, 0, 0 };

extern USBD_HandleTypeDef hUsbDeviceFS;

void Keyboard_Print(uint8_t *data) {

    for (uint8_t i = 0; i < strlen((char*) data); i++) {
        if (ascii_to_scan_code_table[data[i] - 8u] >> 7u) {
            Keyboard_hid.MODIFIER = 0x02; // shift release
        } else {
            Keyboard_hid.MODIFIER = 0x00;
        }
        Keyboard_hid.KEYCODE1 = ascii_to_scan_code_table[data[i] - 8u] & 0x7Fu; // release key
        USBD_HID_SendReport(&hUsbDeviceFS, &Keyboard_hid, sizeof(Keyboard_hid));
        HAL_Delay(KEYBOARD_DELAY);

        Keyboard_hid.MODIFIER = 0x00;  // shift release
        Keyboard_hid.KEYCODE1 = 0x00;  // release key
        Keyboard_hid.KEYCODE2 = 0x00;  // release key
        USBD_HID_SendReport(&hUsbDeviceFS, &Keyboard_hid, sizeof(Keyboard_hid));
        HAL_Delay(KEYBOARD_DELAY);
    }
}

void Keyboard_Write(uint8_t data) {
    if (ascii_to_scan_code_table[data - 8u] >> 7u) {
        Keyboard_hid.MODIFIER = 0x02; // shift release
    } else {
        Keyboard_hid.MODIFIER = 0x00;
    }
    Keyboard_hid.KEYCODE1 = ascii_to_scan_code_table[data - 8u] & 0x7Fu; // release key
    USBD_HID_SendReport(&hUsbDeviceFS, &Keyboard_hid, sizeof(Keyboard_hid));
    HAL_Delay(KEYBOARD_DELAY);

    Keyboard_hid.MODIFIER = 0x00;  // shift release
    Keyboard_hid.KEYCODE1 = 0x00;  // release key
    Keyboard_hid.KEYCODE2 = 0x00;  // release key
    USBD_HID_SendReport(&hUsbDeviceFS, &Keyboard_hid, sizeof(Keyboard_hid));
    HAL_Delay(KEYBOARD_DELAY);
}
