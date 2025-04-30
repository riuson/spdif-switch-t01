#ifndef REMOTE_H
#define REMOTE_H

#include "stdint.h"

enum Pulses {
    PulseLow1 = 1,
    PulseHigh1 = 11,
    PulseLow3 = 3,
    PulseHigh3 = 13,
    PulseLowSync = 9,
    PulseHighSync = 19,
};

enum RCButtons {
    RCButtonNone = 0,
    RCButtonAuto = 1,
    RCButton1 = 2,
    RCButton2 = 3,
    RCButton3 = 4,
};

void remoteInit(void);
void remoteStoreInterval(uint32_t interval, int level);
int remoteGetButton(void);

#endif // REMOTE_H
