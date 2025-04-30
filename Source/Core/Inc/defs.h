#ifndef DEFS_H
#define DEFS_H

typedef enum {
    DetectedSignalSourceNone = 0,
    DetectedSignalSource1 = 1,
    DetectedSignalSource2 = 2,
    DetectedSignalSource3 = 4,
} DetectedSignalSource;

typedef enum {
    SignalSource1,
    SignalSource2,
    SignalSource3,
    SignalSourceAuto,
} SignalSource;

#endif // DEFS_H
