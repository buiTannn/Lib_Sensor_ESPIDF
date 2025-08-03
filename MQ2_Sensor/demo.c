#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "../lib/MQ2_Sensor/mq2.h" 

void app_main(void) {
    printf("Starting MQ2 test...\n");

    mq2_init(34, 10.0, 9.83);  // GPIO34, RL = 10kO, RO = 9.83kO

    while (1) {
        float ppm = readMQ2PPM(SMOKE); 
        printf("SMOKE: %.2f ppm\n", ppm);

        vTaskDelay(pdMS_TO_TICKS(5000)); 
    }
}
