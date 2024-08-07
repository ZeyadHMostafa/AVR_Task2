#include "lcd.h"	

void lcd_send_byte(unsigned char data ){
	// sending upper nibble
	_lcd_send_nibble(data>>4);
	_lcd_enable_pulse(LCD_DELAY_EN);

	_delay_us(LCD_DELAY_NIBBLE);

	//sending lower nibble
	_lcd_send_nibble(data);
	_lcd_enable_pulse(LCD_DELAY_EN);
	
	_delay_ms(2);
}

void lcd_command(unsigned char cmnd ){
	LCD_Port &= ~ (1<<LCD_RS); // RS=0, command reg
	lcd_send_byte(cmnd);
}

void lcd_char( unsigned char data ){
	LCD_Port |= (1<<LCD_RS);//RS=1, data reg
	lcd_send_byte(data);
}

void lcd_init (void){
	
	LCD_Dir = 0xFF; //Pins as Output
	_delay_ms(LCD_DELAY_INIT);
	
	lcd_command(LCD_CMD_INIT);
	lcd_command(LCD_CMD_MODE_4BIT);
	lcd_command(LCD_CMD_MODE_5X7);
	lcd_command(LCD_CMD_CRSR_OFF); //0x0E for testing purposes
	lcd_command(LCD_CMD_CRSR_INC);
	lcd_command(LCD_CMD_SCRN_CLR);
	
	_delay_ms(3);
}

void lcd_string  (char *str){for(int i=0;str[i]!=0;i++){lcd_char (str[i]);}}

void lcd_clear(){

	lcd_command (LCD_CMD_SCRN_CLR);
	_delay_ms(2);
	lcd_command (LCD_CMD_CRSR_HOME);
}