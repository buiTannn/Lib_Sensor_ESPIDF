#include "hcsr04.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"
#include "esp_log.h"
#include "esp_timer.h"
#include <esp_rom_sys.h>

#define TIMEOUT_US 24000

static const char* TAG = "HCSR04";

float getDistance(uint8_t triggerPin, uint8_t echoPin, float maxDistance){
    gpio_set_direction(triggerPin,GPIO_MODE_OUTPUT);
    gpio_set_direction(echoPin,GPIO_MODE_INPUT);

    gpio_set_level(triggerPin,0); // Set trigger pin low
    esp_rom_delay_us(2);// Wait for 2 microseconds
    gpio_set_level(triggerPin,1); // Set trigger pin high
    esp_rom_delay_us(10);   
    gpio_set_level(triggerPin,0); // Set trigger pin low again

    //Timer
    uint64_t start = esp_timer_get_time();
    while (gpio_get_level(echoPin) == 0) {
        if (esp_timer_get_time() - start > TIMEOUT_US) {
            return -1; // timeout
        }
    }
    uint64_t gettime = esp_timer_get_time();
    
    start = esp_timer_get_time();
    while (gpio_get_level(echoPin) == 1) {
        if (esp_timer_get_time() - start > TIMEOUT_US) {
            return -1; // timeout
        }
    }
    uint64_t endtime = esp_timer_get_time();

    uint64_t duration = endtime - gettime;
    float distance = duration / 58;
    if(distance < 2){
        ESP_LOGE(TAG, " SENSOR ERROR , DISTANCE IS LESS THAN 2 CM");
    }
    else if(distance > 400){
        ESP_LOGE(TAG, " SENSOR ERROR , DISTANCE IS MORE THAN 400 CM");
    }
    if(distance > maxDistance){
        return maxDistance;
    }
    return distance;
}
float medianFilter(float *distance){
    float number1 = distance[0];
    float number2 = distance[1];
    float number3 = distance[2];
    if ((number1 >= number2 && number1 <= number3) || (number1 <= number2 && number1 >= number3)) return number1;
    if ((number2 >= number1 && number2 <= number3) || (number2 <= number1 && number2 >= number3)) return number2;
    return number3;
}

float getDistanceMedian(uint8_t triggerPin, uint8_t echoPin, float maxDistance){
    float temp[3] = {0};
    for(int j = 0 ; j < 3; j++){
        temp[j] = getDistance(triggerPin, echoPin, maxDistance);
        vTaskDelay(pdMS_TO_TICKS(60));
    }
        float distanceMedian = medianFilter(temp);

    return distanceMedian;
}