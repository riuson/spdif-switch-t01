#ifndef DEFS_H
#define DEFS_H

typedef enum {
    DetectedSourceNone = 0,
    DetectedSource1 = 1,
    DetectedSource2 = 2,
    DetectedSource3 = 4,
} DetectedSource;

typedef enum {
    RouterSourceNone,
    RouterSource1,
    RouterSource2,
    RouterSource3,
} RouterSource;

typedef enum {
    RCButtonNone = 0,
    RCButtonAuto = 1,
    RCButton1 = 2,
    RCButton2 = 3,
    RCButton3 = 4,
} RCButton;

typedef enum {
    UserSource1,
    UserSource2,
    UserSource3,
    UserSourceAutoKeep,
    UserSourceAutoPriority,
} UserSource;

#endif // DEFS_H
