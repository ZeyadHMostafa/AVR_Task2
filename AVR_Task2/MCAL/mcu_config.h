#ifndef MCU_CONFIG_H_
#define MCU_CONFIG_H_

#define _set_masked(reg,msk,val) reg = (reg&~msk)|val&msk

#include <avr/io.h>

// Timer configurations

#define TMR_PRESCALE_MSK	0x07
#define TMR_PRESCALE_OFF	0
#define TMR_PRESCALE_1		1
#define TMR_PRESCALE_8		2
#define TMR_PRESCALE_64		3
#define TMR_PRESCALE_256	4
#define TMR_PRESCALE_1024	5

#define TMR_MODE_MSK	0x03
#define TMR_MODE_NORMAL 0
#define TMR_MODE_PWM 1
#define TMR_MODE_CTC 2
#define TMR_MODE_FAST_PWM 3

#define TMR_PWM_MODE_MSK (1<<3)
#define TMR_PWM_MODE_TOP 0
#define TMR_PWM_MODE_OCRA 1<<3

void initiate_timer0(unsigned char mode, unsigned char prescale);

#endif