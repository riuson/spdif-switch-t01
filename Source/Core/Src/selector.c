#include "selector.h"

static int stateLocalButton = 0;
static DetectedSource stateDetectedSources;
static UserSource stateUserSource = UserSourceAuto;
static RouterSource stateAutoSelected = RouterSourceNone;

void selectorSetLocalButton(int buttonState) {
    if (stateLocalButton == 0 && buttonState != 0) {
        switch (stateUserSource) {
        case UserSource1: {
            stateUserSource = UserSource2;
            break;
        }
        case UserSource2: {
            stateUserSource = UserSource3;
            break;
        }
        case UserSource3: {
            stateUserSource = UserSourceAuto;
            break;
        }
        case UserSourceAuto: {
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
        stateUserSource = UserSource1;
        break;
    }
    case RCButton2: {
        stateUserSource = UserSource2;
        break;
    }
    case RCButton3: {
        stateUserSource = UserSource3;
        break;
    }
    case RCButtonAuto: {
        stateUserSource = UserSourceAuto;
        break;
    }
    }
}

void selectorSetDetectedSources(DetectedSource sources) { stateDetectedSources = sources; }

RouterSource selectorGetRouterSource(void) {
    switch (stateUserSource) {
    case UserSource1: {
        return RouterSource1;
    }
    case UserSource2: {
        return RouterSource2;
    }
    case UserSource3: {
        return RouterSource3;
    }
    case UserSourceAuto: {
        break;
    }
    }
}