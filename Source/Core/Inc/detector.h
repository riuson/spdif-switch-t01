#ifndef DETECTOR_H
#define DETECTOR_H

#include "defs.h"

void detectorInit(void);
void detectorCheckNextSource(void);
DetectedSignalSource detectorGetFound(void);

#endif // DETECTOR_H
