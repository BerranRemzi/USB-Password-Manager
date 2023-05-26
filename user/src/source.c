/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "Button.h"
#include "Menu.h"
#include "stm32_hal_legacy.h"

__attribute__((section(".noinit"))) volatile uint32_t Bootloader_flag;

#define SYSMEM_RESET_VECTOR            0x1fffC404
#define RESET_TO_BOOTLOADER_MAGIC_CODE 0xDEADBEEF
#define BOOTLOADER_STACK_POINTER       0x1fffC400

/**
 * Function to perform jump to system memory boot from user application
 *
 * Call function when you want to jump to system memory
 */
__attribute__ ((long_call, section (".RamFunc"))) void JumpToBootloader(void) {
#define ApplicationAddress 0x1FFFC400
    typedef void (*pFunction)(void);
    uint32_t JumpAddress = *(__IO uint32_t*) (ApplicationAddress + 4u);
    pFunction Jump_To_Boot = (pFunction) JumpAddress;
    __set_MSP(*(__IO uint32_t*) ApplicationAddress);
    Jump_To_Boot();
}

void Bootloader_Init(void)
{
    if (Bootloader_flag == RESET_TO_BOOTLOADER_MAGIC_CODE) {
        Bootloader_flag = 0;
        JumpToBootloader();
    }
}

void Bootloader_Activate(void) {

#define ADDR_FLASH_PAGE_00    ((uint32_t)0x08000000) /* Base @ of Page 00, 1 Kbytes */
#define ADDR_FLASH_PAGE_31    ((uint32_t)0x08007C00) /* Base @ of Page 31, 1 Kbytes */

#define FLASH_USER_START_ADDR   ADDR_FLASH_PAGE_00   /* Start @ of user Flash area */
#define FLASH_USER_END_ADDR     ADDR_FLASH_PAGE_31 + FLASH_PAGE_SIZE   /* End @ of user Flash area */

    uint32_t PAGEError = 0;

    FLASH_EraseInitTypeDef EraseInitStruct;
    EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
    EraseInitStruct.PageAddress = FLASH_USER_START_ADDR;
    EraseInitStruct.NbPages = 1u;//(FLASH_USER_END_ADDR - FLASH_USER_START_ADDR) / FLASH_PAGE_SIZE;
    HAL_FLASH_Unlock();
    HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError);  //return HAL_OK if success
    HAL_FLASH_Lock();

    __disable_irq();
    HAL_NVIC_SystemReset();
}

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
