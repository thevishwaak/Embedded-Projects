#include<lpc21xx.h>
#include "types.h"
#include "delay.h"
#include "i2c_defines.h"
#include "i2c.h"
#include "lcd.h"
#include "i2c_eeprom.h"
#include "rtc.h"
#include "spi_defines.h"
#include "spi.h"
#include "mcp3208.h"
int main()
{
	u8 sec = 0,min = 9,hours = 9,day = 5,date=5,month=6,year=25;
	//u8 sec,min,hours,day,date,month,year;
	f32 f;
	int temp;
	lcd_config();
	init_i2c();
	Init_SPI0();
	lcd_string("Real weather Mntr");
	delay_s(2);
	lcd_command(0x01);
	rtc_write(sec,min,hours,day,date,month,year);
	while(1)
	{
		f=Read_ADC_MCP3204(0);
		temp = f * 100;
		rtc_read(&sec,&min,&hours,&day,&date,&month,&year);
		lcd_command(0x80);
		lcd_string("Time:");
		lcd_time_display(hours);
		lcd_data(':');
		lcd_time_display(min);
		lcd_data(':');
		lcd_time_display(sec);
		lcd_command(0xc0);
		lcd_string("Celsius:");
		lcd_time_display(temp);
		/*lcd_command(0xC0);         // Move to second line
		lcd_string("D:");
		lcd_time_display(0xC2, date);
		lcd_data('/');
		lcd_time_display(0xC5, month);
		lcd_data('/');
		lcd_time_display(0xC8, year);*/
		delay_ms(1000);
	}
}


/*int main()
{
	u8 sec,min,hours,day,date,month,year;
	lcd_config();
	init_i2c();
	
	lcd_command(0x80);
	lcd_string("T:");
	lcd_command(0x82);
	lcd_data()
	lcd_command(0x40);
	lcd_string("Date: ");
}*/
