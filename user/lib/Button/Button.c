#include "Button.h"
#include "main.h"
#include <stdbool.h>

extern ADC_HandleTypeDef hadc;

#define BUTTON_COUNT 6u
#define BUTTON_ADC_HYSTERESIS 100u
#define BUTTON_DEBOUNCE 4u
#define LONG_PRESS_DEBOUNCE 100u

static uint8_t Button_raw[BUTTON_COUNT];
static uint8_t currentButton = 0u;
static uint8_t prevButton = 0u;
static uint8_t computedButton = 0u;
static uint8_t debounceCounter = 0u;
static uint8_t prevDebounceCounter = 0u;
uint8_t prevPressedButton = false;

Button_Transition_t transition = BUTTON_NO_TRANSITION;

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
    uint8_t pressedButtonCount = 0u;
    uint8_t firstPressedRaw = 0u;
    for (uint8_t i = 0; i < BUTTON_COUNT; i++) {
        if (Button_raw[i] < BUTTON_ADC_HYSTERESIS) {
            pressedButtonCount++;
            if (0u == firstPressedRaw) {
                firstPressedRaw = i + 1u;
            }
        }
    }

    if (1u == pressedButtonCount) {
        currentButton = map[firstPressedRaw - 1u];
    }
    else {
        currentButton = 0u;
    }

    if ((debounceCounter == 0u) || (debounceCounter > BUTTON_DEBOUNCE)) {
        prevDebounceCounter = debounceCounter;
    }

    if ((prevButton == currentButton)) {
        if (debounceCounter < 255u) {
            debounceCounter++;
        }
    }
    else {
        debounceCounter = 0u;
    }

    if ((prevButton > 0u) && (currentButton == 0u)) {
        transition = BUTTON_RELEASED;
        computedButton = prevButton;
    }
    else if ((prevButton == 0u) && (currentButton > 0u)) {
        transition = BUTTON_PRESSED;
        computedButton = currentButton;
    }
    else {
        transition = BUTTON_NO_TRANSITION;
        computedButton = currentButton;
    }

    prevButton = currentButton;
}

uint8_t Button_Get(void) {
    return computedButton;
}

Button_State_t Button_StateGet(void) {
    Button_State_t returnValue = BUTTON_STATE_NOT_PRESSED;
    if (transition == BUTTON_RELEASED)
    { 
        if ((prevDebounceCounter > BUTTON_DEBOUNCE) && (prevDebounceCounter < LONG_PRESS_DEBOUNCE)) {
            returnValue = BUTTON_STATE_SHORT_CLICK;
        }
        else if (prevDebounceCounter > LONG_PRESS_DEBOUNCE) {
            returnValue = BUTTON_STATE_LONG_CLICK;
        }
    } else if (debounceCounter == BUTTON_DEBOUNCE) {
        returnValue = BUTTON_STATE_SHORT_PRESS;
    }

    return returnValue;
}
Button_Transition_t Button_GetTransition() {
    return transition;
}

uint8_t* Button_GetRawValues(void) {
    return Button_raw;
}
