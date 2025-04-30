#include "router.h"

#include "gpio.h"
#include "stm32f0xx.h"

void routerSelect(RouterSource signal) {
    switch (signal) {
    case RouterSource1: {
        LL_GPIO_SetOutputPin(CHANNEL_SELECT_2_GPIO_Port, CHANNEL_SELECT_2_Pin);
        LL_GPIO_SetOutputPin(CHANNEL_SELECT_3_GPIO_Port, CHANNEL_SELECT_3_Pin);
        LL_GPIO_ResetOutputPin(CHANNEL_SELECT_1_GPIO_Port, CHANNEL_SELECT_1_Pin);
        break;
    }
    case RouterSource2: {
        LL_GPIO_SetOutputPin(CHANNEL_SELECT_1_GPIO_Port, CHANNEL_SELECT_1_Pin);
        LL_GPIO_SetOutputPin(CHANNEL_SELECT_3_GPIO_Port, CHANNEL_SELECT_3_Pin);
        LL_GPIO_ResetOutputPin(CHANNEL_SELECT_2_GPIO_Port, CHANNEL_SELECT_2_Pin);
        break;
    }
    case RouterSource3: {
        LL_GPIO_SetOutputPin(CHANNEL_SELECT_1_GPIO_Port, CHANNEL_SELECT_1_Pin);
        LL_GPIO_SetOutputPin(CHANNEL_SELECT_2_GPIO_Port, CHANNEL_SELECT_2_Pin);
        LL_GPIO_ResetOutputPin(CHANNEL_SELECT_3_GPIO_Port, CHANNEL_SELECT_3_Pin);
        break;
    }
    case RouterSourceNone: {
        LL_GPIO_SetOutputPin(CHANNEL_SELECT_1_GPIO_Port, CHANNEL_SELECT_1_Pin);
        LL_GPIO_SetOutputPin(CHANNEL_SELECT_2_GPIO_Port, CHANNEL_SELECT_2_Pin);
        LL_GPIO_SetOutputPin(CHANNEL_SELECT_3_GPIO_Port, CHANNEL_SELECT_3_Pin);
        break;
    }
    }
}
