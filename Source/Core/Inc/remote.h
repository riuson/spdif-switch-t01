#pragma once

#include "defs.h"
#include "stdint.h"

void remoteInit(void);
void remoteStoreInterval(uint32_t interval, int level);
RCButton remoteGetButton(void);
