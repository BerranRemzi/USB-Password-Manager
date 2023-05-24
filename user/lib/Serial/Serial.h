#ifndef SERIAL_H
#define SERIAL_H
#include "stdint.h"

void Serial_Begin(void);
void Serial_End(void);
int8_t Serial_Available(void);
uint8_t Serial_Read(void);
int8_t Serial_peek(void);
void Serial_flush(void);
void Serial_print(uint8_t *data);
void Serial_println(uint8_t *data);
void Serial_write(uint8_t data);

void Serial_Add(uint8_t data);

#endif /* SERIAL_H */
