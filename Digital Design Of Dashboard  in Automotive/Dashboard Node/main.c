#include<lpc21xx.h>
#define sw1 14
#define sw2 15
#define sw3 16
#define led1 1<<17
#define led2 1<<19
#include"lcd.h"
#include"can.h"


int main()
{
	char sw1press = 0;
	char sw2press = 0;
	char sw3press = 0;
	can2_init();
	lcd_config();
	cgram_write(16);
	lcd_command(0x80);
	IODIR0 |= led1 | led2;
	IODIR0 &= ~(sw1|sw2|sw3);
	IOSET0 = led1 | led2;
	lcd_string("CAN Project");
	m1.id = 0x00;
	m1.rtr = 0;
	m1.dlc = 4;
	m1.byteA = 0x00000000;

	while(1)
	{
		if(((IOPIN0>>sw1)&1)==0)
		{
			delay_ms(200);
			while(((IOPIN0>>sw1)&1)==0);
			sw1press = !sw1press;
			if(sw1press)
			{
		  		m1.id = 0x010;
		  		can2_tx(m1);
				lcd_command(0xc0);
				lcd_data(0);
			}
			else
			{
				m1.id = 0x030;
				can2_tx(m1);
				lcd_command(0xc0);
				lcd_data(' ');
			}
				
		}
		if(((IOPIN0>>sw2)&1)==0)
		{
			delay_ms(200);
			while(((IOPIN0>>sw2)&1)==0);
			sw2press = !sw2press;
			if(sw2press)
			{
		  		m1.id = 0x020;
		  		can2_tx(m1);
				lcd_command(0xc2);
				lcd_data(1);
			}
			else
			{
				m1.id = 0x035;
				can2_tx(m1);
				lcd_command(0xc2);
				lcd_data(' ');
			}
		}
		if(((IOPIN0>>sw3)&1)==0)
		{
			delay_ms(200);
			while(((IOPIN0>>sw3)&1)==0);
			sw3press +=1;
			if(sw3press==1)
			{
		  		m1.id = 0x040;
		  		can2_tx(m1);
				lcd_command(0xc4);
				lcd_string("WP SLOW");
			}
			else if(sw3press == 2)
			{
				m1.id = 0x050;
				can2_tx(m1);
				lcd_command(0xc4);
				lcd_string("WP MED ");
			}
			else if(sw3press == 3)
			{
				m1.id = 0x060;
				can2_tx(m1);
				lcd_command(0xc4);
				lcd_string("WP FAST");
			}
			else if(sw3press == 4)
			{
				sw3press =0;
				m1.id = 0x070;
				can2_tx(m1);
				lcd_command(0xc4);
				lcd_string("WP OFF ");			
			}
		}
	}
}
