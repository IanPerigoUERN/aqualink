#ifndef BUZZER_H
#define BUZZER_H

#include "Library/library.h"


#define BUZZER_PIN 21
#define BUZZER_FREQUENCY 200
void pwm_init_buzzer(uint pin);
void update_buzzer(bool bomba_ligada, bool sys_state);



#endif