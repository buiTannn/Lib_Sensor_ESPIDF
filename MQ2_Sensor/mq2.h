#ifndef MQ2_H
#define MQ2_H

#include <stdio.h>
#include "driver/adc.h"

typedef enum {
    CO,
    CH4,
    SMOKE
} GasType;

float readMQ2PPM(GasType gasType);
void mq2_init(int gpio_num, float rl_value, float ro_value);
#endif
