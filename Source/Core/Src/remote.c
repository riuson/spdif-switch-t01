#include "remote.h"
#include "stm32f0xx.h"
#include "string.h"
#include "tim.h"

#define RemoteControlCaptureBufferSize (128)

static uint16_t remoteControlCaptureBufferIndex = 0;
static uint16_t remoteControlCaptureBuffer[RemoteControlCaptureBufferSize];

static uint8_t sequenceButtonAuto[13] = "1111111100FF";
static uint8_t sequenceButton1[13] = "1111111100F1";
static uint8_t sequenceButton2[13] = "1111111100U1";
static uint8_t sequenceButton3[13] = "1111111100UF";

static int checkIntervalInRange(int actual, int expected, int tolerance);

void remoteInit(void) {
    // LL_TIM_IC_Enable(TIM1, LL_TIM_CHANNEL_CH2);
    LL_TIM_EnableIT_CC2(TIM1);
    NVIC_SetPriority(TIM1_CC_IRQn, 0);
    NVIC_EnableIRQ(TIM1_CC_IRQn);
    LL_TIM_CC_EnableChannel(TIM1, LL_TIM_CHANNEL_CH2);
    LL_TIM_EnableIT_CC2(TIM1);
    NVIC_EnableIRQ(TIM1_CC_IRQn);
    LL_TIM_EnableCounter(TIM1);
}

inline void remoteStoreInterval(uint32_t interval, int level) {
    // Capture level of input signal with inversion.
    if (level == 0) {
        interval |= 0x8000;
    }

    remoteControlCaptureBuffer[remoteControlCaptureBufferIndex++] = interval;

    if (remoteControlCaptureBufferIndex >= RemoteControlCaptureBufferSize) {
        remoteControlCaptureBufferIndex = 0;
    }
}

int remoteGetButton(void) {
    // Verify what all intervals are in limits.
    int isAllFits = 1;
    int sync = 0;
    int i;

    for (i = 0; i < RemoteControlCaptureBufferSize; ++i) {
        uint16_t t = remoteControlCaptureBuffer[i] & 0x7FFF;
        if (t >= 300 && t <= 500) {
            continue;
        }
        if (t >= 3 * 300 && t <= 3 * 500) {
            continue;
        }
        if (t >= 32 * 300 && t <= 32 * 500) {
            sync = t;
            continue;
        }

        isAllFits = 0;
        break;
    }

    if (!isAllFits) {
        return isAllFits;
    }

    int sync_div_31 = sync / 31;

    // Verify again and convert.
    int firstSync = -1;
    int lastSync = -1;
    for (i = 0; i < RemoteControlCaptureBufferSize; ++i) {
        uint16_t t = remoteControlCaptureBuffer[i];
        int state = ((t & 0x8000) != 0) ? 1 : 0;
        t &= 0x7FFF;

        if (checkIntervalInRange(t, sync_div_31, 150)) {
            if (firstSync < 0) {
                remoteControlCaptureBuffer[i] = 0;
            } else {
                remoteControlCaptureBuffer[i] = (state != 0) ? PulseHigh1 : PulseLow1;
            }
            continue;
        }
        if (checkIntervalInRange(t, 3 * sync_div_31, 150)) {
            if (firstSync < 0) {
                remoteControlCaptureBuffer[i] = 0;
            } else {
                remoteControlCaptureBuffer[i] = (state != 0) ? PulseHigh3 : PulseLow3;
            }
            continue;
        }
        if (checkIntervalInRange(t, 31 * sync_div_31, 250)) {
            remoteControlCaptureBuffer[i] = (state != 0) ? PulseHighSync : PulseLowSync;
            if (firstSync < 0) {
                firstSync = i;
            } else {
                lastSync = i;
                break;
            }
            continue;
        }

        isAllFits = 0;
        break;
    }

    if (!isAllFits) {
        return isAllFits;
    }

    for (i = lastSync + 1; i < RemoteControlCaptureBufferSize; ++i) {
        remoteControlCaptureBuffer[i] = 0;
    }

    // Decode bits.
    int j;
    uint8_t decodedPacket[12];

    for (i = firstSync + 1, j = 0; i < lastSync && j < 12;) {
        if ((remoteControlCaptureBuffer[i + 0] == PulseHigh1)
            && (remoteControlCaptureBuffer[i + 1] == PulseLow3)
            && (remoteControlCaptureBuffer[i + 2] == PulseHigh1)
            && (remoteControlCaptureBuffer[i + 3] == PulseLow3)) {
            decodedPacket[j] = '0';
        } else if ((remoteControlCaptureBuffer[i + 0] == PulseHigh3)
            && (remoteControlCaptureBuffer[i + 1] == PulseLow1)
            && (remoteControlCaptureBuffer[i + 2] == PulseHigh3)
            && (remoteControlCaptureBuffer[i + 3] == PulseLow1)) {
            decodedPacket[j] = '1';
        } else if ((remoteControlCaptureBuffer[i + 0] == PulseHigh1)
            && (remoteControlCaptureBuffer[i + 1] == PulseLow3)
            && (remoteControlCaptureBuffer[i + 2] == PulseHigh3)
            && (remoteControlCaptureBuffer[i + 3] == PulseLow1)) {
            decodedPacket[j] = 'F';
        } else if ((remoteControlCaptureBuffer[i + 0] == PulseHigh3)
            && (remoteControlCaptureBuffer[i + 1] == PulseLow1)
            && (remoteControlCaptureBuffer[i + 2] == PulseHigh1)
            && (remoteControlCaptureBuffer[i + 3] == PulseLow3)) {
            decodedPacket[j] = 'U';
        } else {
            decodedPacket[j] = '?';
        }
        i += 4;
        j++;
    }

    // Find button code.
    if (memcmp(sequenceButton1, decodedPacket, 12) == 0) {
        return RCButton1;
    } else if (memcmp(sequenceButton2, decodedPacket, 12) == 0) {
        return RCButton2;
    } else if (memcmp(sequenceButton3, decodedPacket, 12) == 0) {
        return RCButton3;
    } else if (memcmp(sequenceButtonAuto, decodedPacket, 12) == 0) {
        return RCButtonAuto;
    }

    return RCButtonNone;
}

static int checkIntervalInRange(int actual, int expected, int tolerance) {
    return ((actual > (expected - tolerance)) && (actual < (expected + tolerance))) ? 1 : 0;
}
