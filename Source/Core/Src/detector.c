#include "detector.h"

#include "gpio.h"
#include "stm32f0xx.h"

static void detectorSelectSource(DetectedSource channel);
static void detectorEnableInput(void);
static void detectorDisableInput(void);

static DetectedSource currentSignalSource = DetectedSource1;
static int interruptsCounter = 0;
static int signalSourceStates[3] = { 0, 0, 0 };

void EXTI4_15_IRQHandler(void);

void detectorInit(void) { detectorEnableInput(); }

void detectorCheckNextSource() {
    int isDetected = (interruptsCounter > 90) ? 1 : 0;

    switch (currentSignalSource) {
        default:
        case DetectedSource1: {
            signalSourceStates[0] = isDetected;
            currentSignalSource = DetectedSource2;
            break;
        }
        case DetectedSource2: {
            signalSourceStates[1] = isDetected;
            currentSignalSource = DetectedSource3;
            break;
        }
        case DetectedSource3: {
            signalSourceStates[2] = isDetected;
            currentSignalSource = DetectedSource1;
            break;
        }
    }

    detectorSelectSource(currentSignalSource);
    interruptsCounter = 0;
    detectorEnableInput();
}

DetectedSource detectorGetFound(void) {
    DetectedSource result = DetectedSourceNone;

    if (signalSourceStates[0] != 0) {
        result |= DetectedSource1;
    }

    if (signalSourceStates[1] != 0) {
        result |= DetectedSource2;
    }

    if (signalSourceStates[2] != 0) {
        result |= DetectedSource3;
    }

    return result;
}

static void detectorSelectSource(DetectedSource source) {
    switch (source) {
        default:
        case DetectedSource1: {
            LL_GPIO_SetOutputPin(MUX_SENS_0_GPIO_Port, MUX_SENS_0_Pin);
            LL_GPIO_SetOutputPin(MUX_SENS_1_GPIO_Port, MUX_SENS_1_Pin);
            break;
        }
        case DetectedSource2: {
            LL_GPIO_ResetOutputPin(MUX_SENS_0_GPIO_Port, MUX_SENS_0_Pin);
            LL_GPIO_SetOutputPin(MUX_SENS_1_GPIO_Port, MUX_SENS_1_Pin);
            break;
        }
        case DetectedSource3: {
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
