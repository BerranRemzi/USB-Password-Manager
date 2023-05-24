/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "Button.h"
#include "Menu.h"

void setup(void) {
    Button_Init();
    Menu_Init();
}

void loop(void) {
	Button_Task();
	Menu_Task();

	//uint8_t *adc = Button_GetRawValues();

	//uint8_t buffer[64];
	//sprintf(buffer, "%3d,%3d,%3d,%3d,%3d,%3d,%2d\n", adc[4], adc[5], adc[3], adc[1], adc[2], adc[0], Button_GetFirstPressed());
	//uint8_t button = Button_GetFirstPressed();

#if 0
    if (button > 0u) {
        sprintf(buffer, "%3d\n", button);
        (void)CDC_Transmit_FS(buffer, strlen((char*) buffer));
    }
#endif

	HAL_GPIO_TogglePin(RED_LED_GPIO_Port, RED_LED_Pin);
	HAL_GPIO_TogglePin(GREEN_LED_GPIO_Port, GREEN_LED_Pin);
	HAL_Delay(10);
}
