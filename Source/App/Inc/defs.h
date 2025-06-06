#pragma once

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
    UserSource1 = 0,
    UserSource2 = 1,
    UserSource3 = 2,
    UserSourceAutoKeep = 3,
    UserSourceAutoPriority = 4,
} UserSource;
