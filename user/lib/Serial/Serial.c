#include "Serial.h"
#include <string.h>

#include "../Keyboard/Keyboard.h"

#define SERIAL_TX_BUFFER_SIZE 16
#define SERIAL_RX_BUFFER_SIZE 16

volatile uint8_t _rx_buffer_head = 0u;
volatile uint8_t _rx_buffer_tail = 0u;
volatile uint8_t _tx_buffer_head = 0u;
volatile uint8_t _tx_buffer_tail = 0u;

unsigned char _rx_buffer[SERIAL_RX_BUFFER_SIZE];
unsigned char _tx_buffer[SERIAL_TX_BUFFER_SIZE];

void Serial_Begin(void) {
}

void Serial_End(void) {
}

int8_t Serial_Available(void) {
    return ((unsigned int) (SERIAL_RX_BUFFER_SIZE + _rx_buffer_head
            - _rx_buffer_tail)) % SERIAL_RX_BUFFER_SIZE;
}

uint8_t Serial_Read(void) {
    // if the head isn't ahead of the tail, we don't have any characters
    if (_rx_buffer_head == _rx_buffer_tail) {
        return -1;
    } else {
        unsigned char c = _rx_buffer[_rx_buffer_tail];
        _rx_buffer_tail = (uint8_t) (_rx_buffer_tail + 1)
                % SERIAL_RX_BUFFER_SIZE;
        return c;
    }
}

int8_t Serial_peek(void) {
    if (_rx_buffer_head == _rx_buffer_tail) {
        return -1;
    } else {
        return _rx_buffer[_rx_buffer_tail];
    }
}

void Serial_flush(void) {
}

void Serial_print(uint8_t *data) {
#ifdef SERIAL
    (void) CDC_Transmit_FS(data, strlen((char*) data));
#else
    Keyboard_Print(data);
#endif

}

void Serial_println(uint8_t *data) {
}

void Serial_write(uint8_t data) {
#ifdef SERIAL
    (void) CDC_Transmit_FS(&data, 1u);
#else
    Keyboard_Write(data);
#endif
}

void Serial_Add(uint8_t c) {
    // No Parity error, read byte and store it in the buffer if there is
    // room
    uint8_t i = (uint8_t) (_rx_buffer_head + 1u) % SERIAL_RX_BUFFER_SIZE;

    // if we should be storing the received character into the location
    // just before the tail (meaning that the head would advance to the
    // current location of the tail), we're about to overflow the buffer
    // and so we don't write the character or advance the head.
    if (i != _rx_buffer_tail) {
        _rx_buffer[_rx_buffer_head] = c;
        _rx_buffer_head = i;
    }
}

