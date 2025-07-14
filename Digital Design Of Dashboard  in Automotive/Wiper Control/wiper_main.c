#include<lpc21xx.h>
#include"can.h"
#include"lcd.h"
#include"fun.h"

void can_rx_intr() __irq
{
   	m1.id=C2RID;
	m1.dlc=(C2RFS>>16)&0xF;
	m1.rtr=(C2RFS>>30)&0x1;
	if(m1.rtr==0){ //if data frame
		m1.byteA=C2RDA;
	}
	C2CMR=(1<<2);
	VICVectAddr = 0;
}

char wiper_sts = 0;
int main()
{
	can2_init();
	lcd_config();
	pwm_config();	
	VICIntSelect = 0;
	VICVectCntl0 = (0x20) | 27;
	VICVectAddr0 = (unsigned long)can_rx_intr;
	VICIntEnable = 1<<27;
	C2IER = 0x01;
	lcd_string("Wiper");
	while(1)
	{
		if(m1.id == 0x040)
		{
			lcd_command(0x01);
			lcd_string("Wp slow");
			wiper_sts = 1;

		}
		else if(m1.id == 0x050)
		{
			lcd_command(0x01);
			lcd_string("Wp mid");
			wiper_sts = 2;
		}
		else if(m1.id == 0x060)
		{
			lcd_command(0x01);
			lcd_string("Wp fast");
			wiper_sts = 3;
		}
		else if(m1.id == 0x070)
		{
			lcd_command(0x01);
			lcd_string("Wp off");
			wiper_sts = 0;
		}
		if(wiper_sts == 1)
		{
			wiper_slow();
		}
		if(wiper_sts == 2)
		{
			wiper_mid();
		}
		if(wiper_sts == 3)
		{
			wiper_fast();
		}
	}
}
