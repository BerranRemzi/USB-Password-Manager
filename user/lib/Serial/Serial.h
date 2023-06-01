#ifndef SERIAL_H
#define SERIAL_H
#include "stdint.h"
#include "stdbool.h"

#ifdef __cplusplus
extern "C" {
#endif

void Serial_begin(void);
void Serial_end(void);
int8_t Serial_available(void);
uint8_t Serial_read(void);
int8_t Serial_peek(void);
void Serial_flush(void);
void Serial_print(const char *data);
void Serial_print_int(int value);
void Serial_println(const char *data);
void Serial_write(uint8_t data);

void Serial_add(uint8_t data);
bool Serial_isDataReady(char *data);

#ifdef __cplusplus
}
#endif

#endif /* SERIAL_H */
