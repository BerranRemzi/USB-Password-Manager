#pragma once

typedef struct {
	const char *key;
	const char *caption;
	const void *pSerialMenuEntry;
	void (*runWhenSelected)();
	void (*runAlways)();
} SerialMenuEntry;

void SerialMenu(char *input);
