
#include "buzzer.h"
#include <pca-hal.h>

void pcaOnInterrupt(PCA_Channel channel, uint16_t HAL_PCA_SEGMENT pulseLength){
}

void buzzer_init(){
    pcaStartCounter(
        PCA_SYSCLK,
        PCA_FREE_RUNNING,
        DISABLE_INTERRUPT,
        BUZZER_PCA_PIN_CONFIG
    );
    pcaConfigureOutput(
        BUZZER_PCA_CHANNEL,
        GPIO_BIDIRECTIONAL_MODE
    );
    pcaStartPwm(
        BUZZER_PCA_CHANNEL,
        PCA_8BIT_PWM,
        PCA_EDGE_NONE,
        1
    );
}

void buzzer_off(){
    pcaStopChannel(BUZZER_PCA_CHANNEL);
    
}

void buzzer_output(uint8_t sample){
    pcaSetDutyCycle(BUZZER_PCA_CHANNEL, sample);
}

