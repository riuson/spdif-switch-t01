#pragma once

#include "defs.h"
#include <stdbool.h>
#include <stdint.h>

bool nvInit(void);
UserSource nvGetState(void);
bool nvSetState(UserSource value);
