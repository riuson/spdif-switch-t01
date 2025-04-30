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
#include "detector.h"
#include "remote.h"
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

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
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

    remoteInit();
    detectorInit();
    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1) {
        /* USER CODE END WHILE */
        /* USER CODE BEGIN 3 */
        detectorCheckNextSource();
        LL_mDelay(500);
        DetectedSource detectedSources = detectorGetFound();
        int button = remoteGetButton();

        switch (button) {
        case RCButton1: {
            LL_GPIO_ResetOutputPin(CHANNEL_SELECT_1_GPIO_Port, CHANNEL_SELECT_1_Pin);
            LL_mDelay(50);
            LL_GPIO_SetOutputPin(CHANNEL_SELECT_1_GPIO_Port, CHANNEL_SELECT_1_Pin);
            break;
        }
        case RCButton2: {
            LL_GPIO_ResetOutputPin(CHANNEL_SELECT_2_GPIO_Port, CHANNEL_SELECT_2_Pin);
            LL_mDelay(50);
            LL_GPIO_SetOutputPin(CHANNEL_SELECT_2_GPIO_Port, CHANNEL_SELECT_2_Pin);
            break;
        }
        case RCButton3: {
            LL_GPIO_ResetOutputPin(CHANNEL_SELECT_3_GPIO_Port, CHANNEL_SELECT_3_Pin);
            LL_mDelay(50);
            LL_GPIO_SetOutputPin(CHANNEL_SELECT_3_GPIO_Port, CHANNEL_SELECT_3_Pin);
            break;
        }
        case RCButtonAuto: {
            LL_GPIO_ResetOutputPin(LED_AUTO_GPIO_Port, LED_AUTO_Pin);
            LL_mDelay(50);
            LL_GPIO_SetOutputPin(LED_AUTO_GPIO_Port, LED_AUTO_Pin);
            break;
        }
        }
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

        int currentPolarity = LL_TIM_IC_GetPolarity(TIM1, LL_TIM_CHANNEL_CH2);

        remoteStoreInterval(interval, currentPolarity == LL_TIM_IC_POLARITY_FALLING);

        prev_capture = current_capture;
        LL_TIM_ClearFlag_CC2(TIM1);

        // Toggle edge detection for next capture
        LL_TIM_IC_SetPolarity(TIM1, LL_TIM_CHANNEL_CH2,
            (currentPolarity == LL_TIM_IC_POLARITY_RISING) ? LL_TIM_IC_POLARITY_FALLING
                                                           : LL_TIM_IC_POLARITY_RISING);
    }
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
