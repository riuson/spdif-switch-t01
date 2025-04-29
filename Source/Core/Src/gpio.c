/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    gpio.c
 * @brief   This file provides code for the configuration
 *          of all used GPIO pins.
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
#include "gpio.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as
 * Analog
 * Input
 * Output
 * EVENT_OUT
 * EXTI
 */
void MX_GPIO_Init(void) {

    LL_EXTI_InitTypeDef EXTI_InitStruct = { 0 };
    LL_GPIO_InitTypeDef GPIO_InitStruct = { 0 };

    /* GPIO Ports Clock Enable */
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOF);
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);

    /**/
    LL_GPIO_ResetOutputPin(CHANNEL_SELECT_1_GPIO_Port, CHANNEL_SELECT_1_Pin);

    /**/
    LL_GPIO_ResetOutputPin(CHANNEL_SELECT_2_GPIO_Port, CHANNEL_SELECT_2_Pin);

    /**/
    LL_GPIO_ResetOutputPin(CHANNEL_SELECT_3_GPIO_Port, CHANNEL_SELECT_3_Pin);

    /**/
    LL_GPIO_ResetOutputPin(MUX_SENS_0_GPIO_Port, MUX_SENS_0_Pin);

    /**/
    LL_GPIO_ResetOutputPin(MUX_SENS_1_GPIO_Port, MUX_SENS_1_Pin);

    /**/
    LL_GPIO_ResetOutputPin(LED_AUTO_GPIO_Port, LED_AUTO_Pin);

    /**/
    GPIO_InitStruct.Pin = CHANNEL_SELECT_1_Pin;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(CHANNEL_SELECT_1_GPIO_Port, &GPIO_InitStruct);

    /**/
    GPIO_InitStruct.Pin = CHANNEL_SELECT_2_Pin;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(CHANNEL_SELECT_2_GPIO_Port, &GPIO_InitStruct);

    /**/
    GPIO_InitStruct.Pin = CHANNEL_SELECT_3_Pin;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(CHANNEL_SELECT_3_GPIO_Port, &GPIO_InitStruct);

    /**/
    GPIO_InitStruct.Pin = MUX_SENS_0_Pin;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(MUX_SENS_0_GPIO_Port, &GPIO_InitStruct);

    /**/
    GPIO_InitStruct.Pin = MUX_SENS_1_Pin;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(MUX_SENS_1_GPIO_Port, &GPIO_InitStruct);

    /**/
    GPIO_InitStruct.Pin = LED_AUTO_Pin;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(LED_AUTO_GPIO_Port, &GPIO_InitStruct);

    /**/
    GPIO_InitStruct.Pin = LL_GPIO_PIN_6;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_DOWN;
    LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /**/
    GPIO_InitStruct.Pin = LL_GPIO_PIN_1;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_DOWN;
    LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /**/
    LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTA, LL_SYSCFG_EXTI_LINE7);

    /**/
    LL_SYSCFG_SetEXTISource(LL_SYSCFG_EXTI_PORTA, LL_SYSCFG_EXTI_LINE10);

    /**/
    LL_GPIO_SetPinPull(SENS_INPUT_GPIO_Port, SENS_INPUT_Pin, LL_GPIO_PULL_NO);

    /**/
    LL_GPIO_SetPinPull(BUTTON_GPIO_Port, BUTTON_Pin, LL_GPIO_PULL_NO);

    /**/
    LL_GPIO_SetPinMode(SENS_INPUT_GPIO_Port, SENS_INPUT_Pin, LL_GPIO_MODE_INPUT);

    /**/
    LL_GPIO_SetPinMode(BUTTON_GPIO_Port, BUTTON_Pin, LL_GPIO_MODE_INPUT);

    /**/
    EXTI_InitStruct.Line_0_31 = LL_EXTI_LINE_7;
    EXTI_InitStruct.LineCommand = ENABLE;
    EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;
    EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_RISING;
    LL_EXTI_Init(&EXTI_InitStruct);

    /**/
    EXTI_InitStruct.Line_0_31 = LL_EXTI_LINE_10;
    EXTI_InitStruct.LineCommand = ENABLE;
    EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;
    EXTI_InitStruct.Trigger = LL_EXTI_TRIGGER_RISING;
    LL_EXTI_Init(&EXTI_InitStruct);
}

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */
