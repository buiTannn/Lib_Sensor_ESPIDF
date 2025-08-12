#ifndef HCSR04_H

#define HCSR04_H
#include <stdint.h>
float getDistanceMedian(uint8_t triggerPin, uint8_t echoPin, float maxDistance);
float medianFilter(float *distance);
float getDistance(uint8_t triggerPin, uint8_t echoPin, float maxDistance);

#endif