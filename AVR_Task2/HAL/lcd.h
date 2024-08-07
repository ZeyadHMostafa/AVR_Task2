#ifndef LCD_H_
#define LCD_H_

	// Pin Definitions

	#ifndef LCD_Dir
	#define LCD_Dir  DDRD
	#endif

	#ifndef LCD_Port
	#define LCD_Port PORTD
	#endif

	#ifndef LCD_RS
	#define LCD_RS PD7
	#endif

	#ifndef LCD_EN
	#define LCD_EN PD5
	#endif

	// delay definitions

	#define LCD_DELAY_INIT 15
	#define LCD_DELAY_NIBBLE 40
	#define LCD_DELAY_EN 20

	// utility definitions

	#define _lcd_enable_pulse(pulse_time) \
		LCD_Port |= (1<<LCD_EN);\
		_delay_us(pulse_time);\
		LCD_Port &= ~ (1<<LCD_EN)

	#define _lcd_send_nibble(data) LCD_Port = (LCD_Port & 0xF0) | (data & 0x0F)

	// configurations

	#define LCD_CMD_INIT		0x33
	#define LCD_CMD_SCRN_CLR	0x01
	#define LCD_CMD_CRSR_OFF 	0x0C
	#define LCD_CMD_CRSR_INC	0x06
	#define LCD_CMD_LINE_2ND	0xC0
	#define LCD_CMD_CRSR_HOME	0x80
	#define LCD_CMD_MODE_4BIT	0x32
	#define LCD_CMD_MODE_5X7	0x28

	// includes

	#define F_CPU 8000000UL
	#include <avr/io.h>
	#include <util/delay.h>


	void lcd_command( unsigned char cmnd );
	void lcd_init (void);
	void lcd_char( unsigned char data );
	void lcd_string (char *str);
	void lcd_clear();

#endif