#include "Serial.h"
#include <string.h>

#define SERIAL
#include "usbd_cdc_if.h"
#include "../Keyboard/Keyboard.h"

#define SERIAL_TX_BUFFER_SIZE 16
#define SERIAL_RX_BUFFER_SIZE 16

volatile uint8_t _rx_buffer_head = 0u;
volatile uint8_t _rx_buffer_tail = 0u;
volatile uint8_t _tx_buffer_head = 0u;
volatile uint8_t _tx_buffer_tail = 0u;

unsigned char _rx_buffer[SERIAL_RX_BUFFER_SIZE];
unsigned char _tx_buffer[SERIAL_TX_BUFFER_SIZE];

void Serial_begin(void) {
}

void Serial_end(void) {
}

int8_t Serial_available(void) {
    return ((unsigned int) (SERIAL_RX_BUFFER_SIZE + _rx_buffer_head
            - _rx_buffer_tail)) % SERIAL_RX_BUFFER_SIZE;
}

uint8_t Serial_read(void) {
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

void Serial_print(const char *data) {
#ifdef SERIAL
    if(data[0] != 255 && data[0] != 0) {
        while(CDC_Transmit_FS((uint8_t*) data, strlen((char*) data)) == USBD_BUSY);
    }
#else
    Keyboard_Print(data);
#endif

}

void Serial_print_int(int value) {
    char temp[8];
    sprintf(temp, "%d", value);
}

void Serial_println(const char *data) {

}

void Serial_write(uint8_t data) {
#ifdef SERIAL
    while(CDC_Transmit_FS(&data, 1u) == USBD_BUSY);
#else
    Keyboard_Write(data);
#endif
}

void Serial_add(uint8_t c) {
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

bool Serial_isDataReady(char *data)
{
  bool returnValue = false;
  static uint8_t position = 0u;
  if (Serial_available() > 0)
  {
    data[position] = Serial_read();
    Serial_write(data[position]);

    switch (data[position])
    {
    case '\n':
      data[position] = '\0';
      position = 0u;
      returnValue = true;
      break;
    case '\r':
      data[position] = '\0';
      break;
    case '\b':
      if (position > 0u)
      {
        position--;
      }
      break;
    default:
      position++;
      if (position > 64)
      {
        position = 0u;
      }
      break;
    }
  }

  return returnValue;
}
