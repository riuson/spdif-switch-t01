#ifndef DETECTOR_H
#define DETECTOR_H

#include "defs.h"

void detectorInit(void);
void detectorCheckNextSource(void);
DetectedSource detectorGetFound(void);

#endif // DETECTOR_H
