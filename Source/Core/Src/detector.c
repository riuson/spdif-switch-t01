#include "detector.h"

#include "gpio.h"
#include "stm32f0xx.h"

static void detectorSelectSource(DetectedSignalSource channel);
static void detectorEnableInput(void);
static void detectorDisableInput(void);

static DetectedSignalSource currentSignalSource = DetectedSignalSource1;
static int interruptsCounter = 0;
static int signalSourceStates[3] = { 0, 0, 0 };

void EXTI4_15_IRQHandler(void);

void detectorInit(void) { detectorEnableInput(); }

void detectorCheckNextSource() {
    int isDetected = (interruptsCounter > 90) ? 1 : 0;

    switch (currentSignalSource) {
    default:
    case DetectedSignalSource1: {
        signalSourceStates[0] = isDetected;
        currentSignalSource = DetectedSignalSource2;
        break;
    }
    case DetectedSignalSource2: {
        signalSourceStates[1] = isDetected;
        currentSignalSource = DetectedSignalSource3;
        break;
    }
    case DetectedSignalSource3: {
        signalSourceStates[2] = isDetected;
        currentSignalSource = DetectedSignalSource1;
        break;
    }
    }

    detectorSelectSource(currentSignalSource);
    interruptsCounter = 0;
    detectorEnableInput();
}

DetectedSignalSource detectorGetFound(void) {
    DetectedSignalSource result = DetectedSignalSourceNone;

    if (signalSourceStates[0] != 0) {
        result |= DetectedSignalSource1;
    }

    if (signalSourceStates[1] != 0) {
        result |= DetectedSignalSource2;
    }

    if (signalSourceStates[2] != 0) {
        result |= DetectedSignalSource3;
    }

    return result;
}

static void detectorSelectSource(DetectedSignalSource source) {
    switch (source) {
    default:
    case DetectedSignalSource1: {
        LL_GPIO_SetOutputPin(MUX_SENS_0_GPIO_Port, MUX_SENS_0_Pin);
        LL_GPIO_SetOutputPin(MUX_SENS_1_GPIO_Port, MUX_SENS_1_Pin);
        break;
    }
    case DetectedSignalSource2: {
        LL_GPIO_ResetOutputPin(MUX_SENS_0_GPIO_Port, MUX_SENS_0_Pin);
        LL_GPIO_SetOutputPin(MUX_SENS_1_GPIO_Port, MUX_SENS_1_Pin);
        break;
    }
    case DetectedSignalSource3: {
        LL_GPIO_ResetOutputPin(MUX_SENS_0_GPIO_Port, MUX_SENS_0_Pin);
        LL_GPIO_ResetOutputPin(MUX_SENS_1_GPIO_Port, MUX_SENS_1_Pin);
        break;
    }
    }
}

static void detectorEnableInput(void) {
    NVIC_SetPriority(EXTI4_15_IRQn, 1);
    NVIC_EnableIRQ(EXTI4_15_IRQn);
}

static void detectorDisableInput(void) { NVIC_DisableIRQ(EXTI4_15_IRQn); }

void EXTI4_15_IRQHandler(void) {
    if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_7)) {
        if (interruptsCounter < 100) {
            ++interruptsCounter;
        } else {
            detectorDisableInput();
        }

        LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_7);
    }
}
