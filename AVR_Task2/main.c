#define F_CPU 8000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include <stdlib.h>

#include "MCAL/mcu_config.h"
#include "HAL/LCD.h"

#define FAN_PIN 3 //B3
#define TEMP_SENSOR_PIN 0 //C0
#define MODE_PIN 1 //B1

#define AUTO_TURNOFF_INTERVAL 10
#define VOLT_TO_CELCIUS 100.0/256.0
#define TEMP_THRESH 30

#define AUTO_MODE (((~PINB)>> MODE_PIN)&1) // AUTO_MODE 0: turn off below threshold, AUTO_MODE 1: turn off after timer

unsigned char fan_state = 0; // 0: off, 1: on, 2: idle
unsigned int turnoff_counter = 0;
unsigned int temp_value = 0;
char temp_str[4];

void control_fan(unsigned char new_state){
	// prevent constant calling of function
	// just in case it gets more complex
	
	if (new_state == fan_state){return;}
	fan_state = new_state;
	
	// actual function should probably call from HAL
	
	if (fan_state==1){
		PORTB |= (1 << FAN_PIN);
	}else{
		PORTB &= ~(1 << FAN_PIN);
	}
}

void print_temprature(unsigned char value){
	lcd_clear();
	lcd_string("T:");
	itoa(value,temp_str,10);
	lcd_string(temp_str);
	lcd_char(0xDF); // DEGREE symbol
	lcd_char('C');
	
}

int main(void) {
	// Initialize I/O ports
	DDRB &= ~(1 << MODE_PIN); // Set MODE_PIN as input
	PORTB |= (1 << MODE_PIN); // Set MODE_PIN as pull-up
	DDRB |= (1 << FAN_PIN); // Set FAN_PIN as output
	DDRC &= ~(1 << TEMP_SENSOR_PIN); // Set TEMP_SENSOR_PIN as input
	
	// prepare LCD
	lcd_init();
	
	// prepare timer
	TIMSK0 |= (1 << OCIE0A); // Enable timer interrupt
	OCR0A = 249;
	initiate_timer0(TMR_MODE_CTC,TMR_PRESCALE_256);
	
	// prepare temperature sensor
	ADMUX |= (1 << REFS0) | (1 << ADLAR); // AVCC and ADC0
	ADCSRA |= (1 << ADEN) | (1 << ADPS2) |  (1 << ADIE);
	sei();
	ADCSRA |= 1 << ADSC; // start conversion
	
	while (1)
	{
		// Convert temperature to Celsius
		unsigned char temperature = temp_value * VOLT_TO_CELCIUS;

		// control fan
		if (temperature >= TEMP_THRESH){
			if (AUTO_MODE==0){
				turnoff_counter = 0;
			}
			if (fan_state==0 || ((AUTO_MODE==0)&&(fan_state==2))){
				control_fan(1);
			}
		}else{
			if (AUTO_MODE==0){
				control_fan(0);
			}
		}
		
		// Display temperature on LCD
		print_temprature(temperature);
		
		_delay_ms(200);
	}
}

// Timer interrupt handler for automatic turnoff interval
ISR(TIMER0_COMPA_vect) {
	/*
	tick time = 0.008
	ticks per second = 125
	ticks for full time = 125*delay in seconds
	*/
	if (fan_state == 1 && AUTO_MODE){
		turnoff_counter++;
		if (turnoff_counter >= 125*AUTO_TURNOFF_INTERVAL){
			control_fan(2);
		}
	}
}

// ISR for ADC temperature update
ISR(ADC_vect) {
	temp_value = ADCH;
	ADCSRA |= 1 << ADSC;
}