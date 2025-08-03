#include "mq2.h"
#include <math.h>
#include "driver/adc.h"
#include "esp_adc_cal.h"
#include <string.h>

#define ADC_MAX 4095.0
#define ADC_VREF 3.3 

static adc1_channel_t mq2_channel;
static float mq2_rl;
static float mq2_ro;

adc1_channel_t get_adc1_channel_from_gpio(int gpio_num) {
    switch (gpio_num) {
        case 36: return ADC1_CHANNEL_0;
        case 37: return ADC1_CHANNEL_1;
        case 38: return ADC1_CHANNEL_2;
        case 39: return ADC1_CHANNEL_3;
        case 32: return ADC1_CHANNEL_4;
        case 33: return ADC1_CHANNEL_5;
        case 34: return ADC1_CHANNEL_6;
        case 35: return ADC1_CHANNEL_7;
        default:
            printf("GPIO %d can't use ADC", gpio_num);
            return -1; 
    }
}
void mq2_init(int gpio_num, float rl_value, float ro_value) {
    adc1_channel_t channel = get_adc1_channel_from_gpio(gpio_num);
    if (channel < 0) {
        printf(" Can't get adc channel %d\n", gpio_num);
        return;
    }
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(channel, ADC_ATTEN_DB_12);
    mq2_channel = channel;
    mq2_rl = rl_value;
    mq2_ro = ro_value;
}


float calculateResistance(float rawADC, float RL) {
    float voltage = rawADC * ADC_VREF / ADC_MAX;
    if (voltage <= 0.01) return 1000000.0;
    return ((5.0 - voltage) / voltage) * RL;
}

float calculatePPM(float ratio, GasType gasType) {
    float log_ppm = 0;
    switch (gasType) {
        case CO:
            log_ppm = -4.107 * log10(ratio) + 5.172;
            break;
        case CH4:
            log_ppm = -2.260 * log10(ratio) + 3.379;
            break;
        case SMOKE:
            log_ppm = -1.205 * log10(ratio) + 2.957;
            break;
        default:
            return -1;
    }
    return pow(10, log_ppm);
}

float readMQ2PPM(GasType gasType) {
    int raw = adc1_get_raw(mq2_channel);
    float rs = calculateResistance((float)raw, mq2_rl);
    float ratio = rs / mq2_ro;
    float ppm = calculatePPM(ratio, gasType);
    return ppm;
}



