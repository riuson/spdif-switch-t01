#pragma once

#include "defs.h"
#include <stdint.h>

void selectorSetLocalButton(int buttonState);
void selectorSetRCButton(RCButton button);
void selectorSetDetectedSources(DetectedSource sources);
RouterSource selectorGetRouterSource(void);

UserSource selectorGetUserSelection(void);
void selectorSetUserSelection(UserSource value);
