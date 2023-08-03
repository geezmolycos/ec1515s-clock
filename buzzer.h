
#ifndef _BUZZER_H
#define _BUZZER_H

#include "project-defs.h"

#define BUZZER_PCA_PIN_CONFIG 1
#define BUZZER_PCA_CHANNEL 0

void buzzer_init();
void buzzer_output(uint8_t sample);

#endif // _BUZZER_H
