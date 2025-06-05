#include "app.h"

#include "stm32f0xx.h"

#include "gpio.h"
#include "tim.h"
#include "usart.h"

#include "detector.h"
#include "nv.h"
#include "pins.h"
#include "remote.h"
#include "router.h"
#include "selector.h"

void app(void) {
    routerSelect(RouterSourceNone);
    remoteInit();
    detectorInit();

    // Pre-detect sources.
    detectorCheckNextSource();
    LL_mDelay(100);
    detectorCheckNextSource();
    LL_mDelay(100);
    detectorCheckNextSource();
    LL_mDelay(100);

    // Infinite loop
    while (1) {
        detectorCheckNextSource();
        LL_mDelay(100);
        DetectedSource detectedSources = detectorGetFound();
        RCButton remoteButton = remoteGetButton();
        selectorSetRCButton(remoteButton);
        selectorSetDetectedSources(detectedSources);
        selectorSetLocalButton(!LL_GPIO_IsInputPinSet(BUTTON_GPIO_Port, BUTTON_Pin));
        routerSelect(selectorGetRouterSource());
    }
}
