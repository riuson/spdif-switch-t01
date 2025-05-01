#include "selector.h"
#include "gpio.h"

static int stateLocalButton = 0;
static DetectedSource stateDetectedSources;
static UserSource stateUserSource = UserSourceAuto;
static RouterSource stateLastSelectedSource = RouterSourceNone;

static int selectorCheckSourceAvailable(RouterSource source);
static int selectorGetFirstAvailableSource(void);
static void selectorHighlightAutoSource(int enable);

void selectorSetLocalButton(int buttonState) {
    if (stateLocalButton == 0 && buttonState != 0) {
        switch (stateUserSource) {
            case UserSource1: {
                selectorHighlightAutoSource(0);
                stateUserSource = UserSource2;
                break;
            }
            case UserSource2: {
                selectorHighlightAutoSource(0);
                stateUserSource = UserSource3;
                break;
            }
            case UserSource3: {
                selectorHighlightAutoSource(1);
                stateUserSource = UserSourceAuto;
                break;
            }
            case UserSourceAuto: {
                selectorHighlightAutoSource(0);
                stateUserSource = UserSource1;
                break;
            }
        }
    }
    stateLocalButton = buttonState;
}

void selectorSetRCButton(RCButton button) {
    switch (button) {
        case RCButton1: {
            selectorHighlightAutoSource(0);
            stateUserSource = UserSource1;
            break;
        }
        case RCButton2: {
            selectorHighlightAutoSource(0);
            stateUserSource = UserSource2;
            break;
        }
        case RCButton3: {
            selectorHighlightAutoSource(0);
            stateUserSource = UserSource3;
            break;
        }
        case RCButtonAuto: {
            selectorHighlightAutoSource(1);
            stateUserSource = UserSourceAuto;
            break;
        }
        default: {
            break;
        }
    }
}

void selectorSetDetectedSources(DetectedSource sources) { stateDetectedSources = sources; }

RouterSource selectorGetRouterSource(void) {
    switch (stateUserSource) {
        case UserSource1: {
            // Manual selection.
            stateLastSelectedSource = RouterSource1;
            return RouterSource1;
        }
        case UserSource2: {
            // Manual selection.
            stateLastSelectedSource = RouterSource2;
            return RouterSource2;
        }
        case UserSource3: {
            // Manual selection.
            stateLastSelectedSource = RouterSource3;
            return RouterSource3;
        }
        default:
        case UserSourceAuto: {
            // Auto selection.
            if (selectorCheckSourceAvailable(stateLastSelectedSource) == 0) {
                stateLastSelectedSource = selectorGetFirstAvailableSource();
            }
            return stateLastSelectedSource;
        }
    }
}

static int selectorCheckSourceAvailable(RouterSource source) {
    switch (source) {
        case RouterSource1: {
            return ((stateDetectedSources & DetectedSource1) != DetectedSourceNone) ? 1 : 0;
        }
        case RouterSource2: {
            return ((stateDetectedSources & DetectedSource2) != DetectedSourceNone) ? 1 : 0;
        }
        case RouterSource3: {
            return ((stateDetectedSources & DetectedSource3) != DetectedSourceNone) ? 1 : 0;
        }
        default:
            return 0;
    }
}

static int selectorGetFirstAvailableSource(void) {
    if ((stateDetectedSources & DetectedSource1) != DetectedSourceNone) {
        return RouterSource1;
    } else if ((stateDetectedSources & DetectedSource2) != DetectedSourceNone) {
        return RouterSource2;
    } else if ((stateDetectedSources & DetectedSource2) != DetectedSourceNone) {
        return RouterSource3;
    }

    return RouterSourceNone;
}

static void selectorHighlightAutoSource(int enable) {
    if (enable != 0) {
        LL_GPIO_ResetOutputPin(LED_AUTO_GPIO_Port, LED_AUTO_Pin);
    } else {
        LL_GPIO_SetOutputPin(LED_AUTO_GPIO_Port, LED_AUTO_Pin);
    }
}