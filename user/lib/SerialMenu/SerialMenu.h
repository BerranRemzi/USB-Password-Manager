#ifndef SERIAL_MENU_H
#define SERIAL_MENU_H

#include "stdint.h"

typedef struct {
	const char *key;
	const char *caption;
	const void *pSerialMenuEntry;
	void (*runWhenSelected)(void);
	void (*runAlways)(void);
} SerialMenuEntry;

#ifdef __cplusplus
extern "C" {
#endif

void SerialMenu(char *input);

#ifdef __cplusplus
}
#endif

#endif /* SERIAL_MENU_H */
