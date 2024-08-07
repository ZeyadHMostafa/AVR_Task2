#include "mcu_config.h"

void initiate_timer0(unsigned char mode, unsigned char prescale){
	_set_masked(TCCR0A,TMR_MODE_MSK,mode);
	_set_masked(TCCR0B,TMR_PWM_MODE_MSK,mode);
	_set_masked(TCCR0B,TMR_PRESCALE_MSK,prescale);
}