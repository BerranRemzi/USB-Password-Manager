#include "Button.h"
#include "main.h"
#include <stdbool.h>

extern ADC_HandleTypeDef hadc;
#define BUTTON_COUNT 6u
#define BUTTON_PRESSSED_VALUE 75u
#define BUTTON_DEBOUNCE 4u

static uint8_t Button_raw[BUTTON_COUNT];
static uint8_t pressedButton = 0u;
static uint8_t prevButton = 0u;
static uint8_t debounceCounter = 0u;

void ADC_Select_Channel(uint32_t ch) {
    ADC_ChannelConfTypeDef conf = {
            .Channel = ch, .Rank = 1,
            .SamplingTime =
            ADC_SAMPLETIME_28CYCLES_5,
    };

    if (HAL_ADC_ConfigChannel(&hadc, &conf) != HAL_OK) {
        Error_Handler();
    }
}

void Adc_ReadAllChannels(uint8_t *data) {
    HAL_ADC_Start(&hadc);
    for (int i = 0; i < 6; i++) {

        HAL_ADC_PollForConversion(&hadc, HAL_MAX_DELAY);
        data[i] = HAL_ADC_GetValue(&hadc);

    }
    HAL_ADC_Stop(&hadc);
}

void HAL_ModeChange(ADC_HandleTypeDef *hadc, uint32_t mode, uint32_t pull) {
    GPIO_InitTypeDef GPIO_InitStruct = {
            0 };
    if (hadc->Instance == ADC1) {
        /* USER CODE BEGIN ADC1_MspInit 0 */

        /* USER CODE END ADC1_MspInit 0 */
        /* Peripheral clock enable */
        __HAL_RCC_ADC1_CLK_ENABLE();

        __HAL_RCC_GPIOA_CLK_ENABLE();
        /**ADC GPIO Configuration
         PA0     ------> ADC_IN0
         PA1     ------> ADC_IN1
         PA3     ------> ADC_IN3
         PA4     ------> ADC_IN4
         PA5     ------> ADC_IN5
         PA7     ------> ADC_IN7
         */
        GPIO_InitStruct.Pin = BTN_6_Pin | BTN_4_Pin | BTN_5_Pin | BTN_3_Pin
                | BTN_1_Pin | BTN_2_Pin;
        GPIO_InitStruct.Mode = mode;
        GPIO_InitStruct.Pull = pull;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* USER CODE BEGIN ADC1_MspInit 1 */

        /* USER CODE END ADC1_MspInit 1 */
    }

}

void Button_Init(void) {

}

void Button_Task(void) {

    const uint8_t map[BUTTON_COUNT] = {
            6,
            4,
            5,
            3,
            1,
            2
    };

    HAL_ModeChange(&hadc, GPIO_MODE_INPUT, GPIO_PULLUP);
    HAL_ModeChange(&hadc, GPIO_MODE_ANALOG, GPIO_NOPULL);
    HAL_Delay(1);

    Adc_ReadAllChannels(Button_raw);
    uint8_t pressedCount = 0u;
    uint8_t firstPressed = 0u;
    for (uint8_t i = 0; i < BUTTON_COUNT; i++) {
        if (Button_raw[i] < BUTTON_PRESSSED_VALUE) {
            pressedCount++;
            if (0u == firstPressed) {
                firstPressed = i + 1u;
            }
        }
    }

    if (1u == pressedCount/* && debounceCounter == 4u*/) {
        pressedButton = map[firstPressed - 1u];
    } else {
        pressedButton = 0u;
    }

    if ((prevButton == firstPressed) && (pressedButton > 0u)) {
        debounceCounter++;
    } else {
        debounceCounter = 0u;
    }

    prevButton = firstPressed;
}

uint8_t Button_Get(void) {
    return pressedButton;
}

uint8_t Button_StateGet(void) {
    Button_t returnValue = BUTTON_STATE_NOT_PRESSED;

    if (4u == debounceCounter) {
        returnValue = BUTTON_STATE_PRESSED;
    } else if (100u == debounceCounter) {
        returnValue = BUTTON_STATE_LONG_PRESSED;
    }
    return returnValue;
}

uint8_t* Button_GetRawValues(void) {
    return Button_raw;
}
