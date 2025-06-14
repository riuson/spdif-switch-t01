/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    tim.c
 * @brief   This file provides code for the configuration
 *          of the TIM instances.
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
#include "tim.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* TIM1 init function */
void MX_TIM1_Init(void) {

    /* USER CODE BEGIN TIM1_Init 0 */

    /* USER CODE END TIM1_Init 0 */

    LL_TIM_InitTypeDef TIM_InitStruct = { 0 };

    LL_GPIO_InitTypeDef GPIO_InitStruct = { 0 };

    /* Peripheral clock enable */
    LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_TIM1);

    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
    /**TIM1 GPIO Configuration
    PA9   ------> TIM1_CH2
    */
    GPIO_InitStruct.Pin = REMOTE_INPUT_Pin;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_DOWN;
    GPIO_InitStruct.Alternate = LL_GPIO_AF_2;
    LL_GPIO_Init(REMOTE_INPUT_GPIO_Port, &GPIO_InitStruct);

    /* USER CODE BEGIN TIM1_Init 1 */

    /* USER CODE END TIM1_Init 1 */
    TIM_InitStruct.Prescaler = (SystemCoreClock / 1000000) - 1; // set 1 us resolution
    TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
    TIM_InitStruct.Autoreload = 65535;
    TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
    TIM_InitStruct.RepetitionCounter = 0;
    LL_TIM_Init(TIM1, &TIM_InitStruct);
    LL_TIM_DisableARRPreload(TIM1);
    LL_TIM_SetClockSource(TIM1, LL_TIM_CLOCKSOURCE_INTERNAL);
    LL_TIM_SetTriggerOutput(TIM1, LL_TIM_TRGO_RESET);
    LL_TIM_DisableMasterSlaveMode(TIM1);
    LL_TIM_IC_SetActiveInput(TIM1, LL_TIM_CHANNEL_CH2, LL_TIM_ACTIVEINPUT_DIRECTTI);
    LL_TIM_IC_SetPrescaler(TIM1, LL_TIM_CHANNEL_CH2, LL_TIM_ICPSC_DIV1);
    LL_TIM_IC_SetFilter(TIM1, LL_TIM_CHANNEL_CH2, LL_TIM_IC_FILTER_FDIV1);
    LL_TIM_IC_SetPolarity(TIM1, LL_TIM_CHANNEL_CH2, LL_TIM_IC_POLARITY_BOTHEDGE);
    /* USER CODE BEGIN TIM1_Init 2 */

    /* USER CODE END TIM1_Init 2 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
