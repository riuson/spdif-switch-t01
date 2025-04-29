/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "gpio.h"
#include "tim.h"
#include "usart.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stm32f0xx.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
#define RemoteControlCaptureBufferSize (128)
uint16_t remoteControlCaptureBufferIndex;
uint16_t remoteControlCaptureBuffer[RemoteControlCaptureBufferSize];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
int decodeRemote(const uint16_t* timings, int count);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {

    /* USER CODE BEGIN 1 */

    /* USER CODE END 1 */

    /* MCU Configuration--------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_SYSCFG);
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

    /* SysTick_IRQn interrupt configuration */
    NVIC_SetPriority(SysTick_IRQn, 3);

    /* USER CODE BEGIN Init */
    remoteControlCaptureBufferIndex = 0;
    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();

    /* USER CODE BEGIN SysInit */

    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_TIM1_Init();
    // MX_USART1_UART_Init();
    /* USER CODE BEGIN 2 */
    LL_GPIO_SetOutputPin(CHANNEL_SELECT_1_GPIO_Port, CHANNEL_SELECT_1_Pin);
    LL_GPIO_SetOutputPin(CHANNEL_SELECT_2_GPIO_Port, CHANNEL_SELECT_2_Pin);
    LL_GPIO_SetOutputPin(CHANNEL_SELECT_3_GPIO_Port, CHANNEL_SELECT_3_Pin);

    // LL_TIM_IC_Enable(TIM1, LL_TIM_CHANNEL_CH2);
    LL_TIM_EnableIT_CC2(TIM1);
    NVIC_SetPriority(TIM1_CC_IRQn, 0);
    NVIC_EnableIRQ(TIM1_CC_IRQn);
    LL_TIM_CC_EnableChannel(TIM1, LL_TIM_CHANNEL_CH2);
    LL_TIM_EnableIT_CC2(TIM1);
    NVIC_EnableIRQ(TIM1_CC_IRQn);
    LL_TIM_EnableCounter(TIM1);

    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1) {
        /* USER CODE END WHILE */
        /* USER CODE BEGIN 3 */
        LL_mDelay(500);

        LL_TIM_DisableCounter(TIM1);
        if (decodeRemote(remoteControlCaptureBuffer, RemoteControlCaptureBufferSize) != 0) {
            LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_5);
            LL_mDelay(50);
            LL_GPIO_SetOutputPin(GPIOA, LL_GPIO_PIN_5);
        }
        LL_TIM_EnableCounter(TIM1);
    }
    /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
    LL_FLASH_SetLatency(LL_FLASH_LATENCY_1);
    while (LL_FLASH_GetLatency() != LL_FLASH_LATENCY_1) { }
    LL_RCC_HSI_Enable();

    /* Wait till HSI is ready */
    while (LL_RCC_HSI_IsReady() != 1) { }
    LL_RCC_HSI_SetCalibTrimming(16);
    LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI_DIV_2, LL_RCC_PLL_MUL_8);
    LL_RCC_PLL_Enable();

    /* Wait till PLL is ready */
    while (LL_RCC_PLL_IsReady() != 1) { }
    LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
    LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
    LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

    /* Wait till System clock is ready */
    while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL) { }
    LL_Init1msTick(32000000);
    LL_SetSystemCoreClock(32000000);
    LL_RCC_SetUSARTClockSource(LL_RCC_USART1_CLKSOURCE_PCLK1);
}

/* USER CODE BEGIN 4 */
void TIM1_CC_IRQHandler(void) {
    if (LL_TIM_IsActiveFlag_CC2(TIM1)) {
        static uint32_t prev_capture = 0;
        uint32_t current_capture = LL_TIM_IC_GetCaptureCH2(TIM1);
        uint32_t interval = current_capture - prev_capture;

        // Handle overflow (0xFFFF -> 0)
        if (interval > 0x7FFF) {
            interval += 0x10000;
        }

        remoteControlCaptureBuffer[remoteControlCaptureBufferIndex++] = interval;

        if (remoteControlCaptureBufferIndex >= RemoteControlCaptureBufferSize) {
            remoteControlCaptureBufferIndex = 0;
        }

        prev_capture = current_capture;
        LL_TIM_ClearFlag_CC2(TIM1);

        // Toggle edge detection for next capture
        LL_TIM_IC_SetPolarity(TIM1, LL_TIM_CHANNEL_CH2,
            (LL_TIM_IC_GetPolarity(TIM1, LL_TIM_CHANNEL_CH2) == LL_TIM_IC_POLARITY_RISING)
                ? LL_TIM_IC_POLARITY_FALLING
                : LL_TIM_IC_POLARITY_RISING);
    }
}

int checkIntervalInRange(int actual, int expected, int tolerance) {
    return ((actual > (expected - tolerance)) && (actual < (expected + tolerance))) ? 1 : 0;
}

int decodeRemote(const uint16_t* timings, int count) {
    // Verify for all intervals in limits.
    int isAllFits = 1;
    int pulse1 = 0;
    int pulse3 = 0;
    int sync = 0;
    int i;

    for (i = 0; i < count; ++i) {
        uint16_t t = timings[i];
        if (t >= 300 && t <= 500) {
            // pulse1 = t;
            continue;
        }
        if (t >= 3 * 300 && t <= 3 * 500) {
            // pulse3 = t;
            continue;
        }
        if (t >= 32 * 300 && t <= 32 * 500) {
            sync = t;
            continue;
        }

        isAllFits = 0;
        break;
    }

    if (!isAllFits) {
        return isAllFits;
    }

    int sync_div_31 = sync / 31;

    for (i = 0; i < count; ++i) {
        uint16_t t = timings[i];
        if (checkIntervalInRange(t, sync_div_31, 150)) {
            pulse1 = t;
            continue;
        }
        if (checkIntervalInRange(t, 3 * sync_div_31, 150)) {
            pulse3 = t;
            continue;
        }
        if (checkIntervalInRange(t, 31 * sync_div_31, 250)) {
            sync = t;
            continue;
        }

        isAllFits = 0;
        break;
    }

    if (!isAllFits) {
        return isAllFits;
    }

    return isAllFits;
}

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while (1) { }
    /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t* file, uint32_t line) {
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
