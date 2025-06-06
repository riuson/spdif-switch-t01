#include "app.h"

#include "stm32f0xx.h"
#include <stddef.h>

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
    bool nvOk = nvInit();

    // Indicate NV error.
    if (!nvOk) {
        for (size_t i = 0; i < 10; ++i) {
            LL_GPIO_ResetOutputPin(LED_AUTO_GPIO_Port, LED_AUTO_Pin);
            LL_mDelay(100);
            LL_GPIO_SetOutputPin(LED_AUTO_GPIO_Port, LED_AUTO_Pin);
            LL_mDelay(100);
        }
    }

    UserSource savedUserSource = nvGetState();
    UserSource lastUserSource = savedUserSource;
    selectorSetUserSelection(savedUserSource);

    // Pre-detect sources.
    detectorCheckNextSource();
    LL_mDelay(100);
    detectorCheckNextSource();
    LL_mDelay(100);
    detectorCheckNextSource();
    LL_mDelay(100);

    size_t loopCounter = 0;

    // Infinite loop
    while (true) {
        detectorCheckNextSource();
        LL_mDelay(100);
        DetectedSource detectedSources = detectorGetFound();
        RCButton remoteButton = remoteGetButton();
        selectorSetRCButton(remoteButton);
        selectorSetDetectedSources(detectedSources);
        selectorSetLocalButton(!LL_GPIO_IsInputPinSet(BUTTON_GPIO_Port, BUTTON_Pin));
        routerSelect(selectorGetRouterSource());

        UserSource newUserSource = selectorGetUserSelection();

        // If state changed, start delay.
        if (newUserSource != lastUserSource) {
            lastUserSource = newUserSource;
            loopCounter = 1;
        }

        // If delay started, count cycles.
        if (loopCounter > 0) {
            loopCounter++;
        }

        // Then cycles elapsed, write changes and stop counter.
        if (loopCounter > 200) {
            loopCounter = 0;

            if (nvOk) {
                if (newUserSource != savedUserSource) {
                    nvOk = nvSetState(newUserSource);
                    savedUserSource = newUserSource;
                }
            }
        }
    }
}
