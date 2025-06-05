#pragma once

#include "defs.h"
#include <stdint.h>

void nvInit(void);
UserSource nvGetState(void);
void nvSetState(UserSource value);
