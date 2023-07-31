
#include "sensor.h"
#include <adc-comp-hal.h>

void sensor_init(){
    adcInitialise(ADC_ALIGN_RIGHT, DISABLE_INTERRUPT);
}

void sensor_on(){
    adcPowerOn();
    adcConfigureChannel(SENSOR_LIGHT_CHANNEL);
    adcConfigureChannel(SENSOR_TEMP_CHANNEL);
}

void sensor_off(){
    adcPowerOff();
    P1ASF = 0x00;
}

uint16_t sensor_get_light(){
    return adcBlockingRead(SENSOR_LIGHT_CHANNEL);
}

uint16_t sensor_get_temp(){
    return adcBlockingRead(SENSOR_TEMP_CHANNEL);
}
