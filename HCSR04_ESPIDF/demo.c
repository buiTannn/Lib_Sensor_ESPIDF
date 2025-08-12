#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "../lib/HCSR04_ESPIDF/hcsr04.h"

void app_main(void) {
    while(1){
    float distance = getDistance(5, 26,50); //Distance in cm
    float distanceMedian = getDistanceMedian(5, 26, 50); //Filtered distance
    printf("Distance: %.2f cm\n", distance);
    printf("Median Distance: %.2f cm\n", distanceMedian);
    vTaskDelay(pdMS_TO_TICKS(1000));
    }

}