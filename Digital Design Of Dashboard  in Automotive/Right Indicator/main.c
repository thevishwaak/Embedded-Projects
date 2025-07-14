#include<lpc21xx.h>
#include"lcd.h"
#include"can.h"

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

int led[3] = {1<<17,1<<18,1<<19};
int led_sts = 0;
int main()
{		
		int i,k;
		lcd_config();
		can2_init();
		
		VICIntSelect = 0;
		VICVectCntl0 = (0x20) | 27;
		VICVectAddr0 = (unsigned long)can_rx_intr;
		VICIntEnable = 1<<27;
		C2IER = 0x01;
		IODIR0 = led[0]|led[1]|led[2];
		IOSET0 = led[0]|led[1]|led[2];
		cgram_write(8);
		lcd_string("Right Indicator");
		while(1)
		{
			if(m1.id == 0x020)
			{	
   				led_sts = 1;
			}
			else if (m1.id == 0x035)
			{
				led_sts = 0;
				IOSET0 = led[0]|led[1]|led[2];
			}
 
			if(led_sts)
			{
				for(i=0xc0,k=0;(i<0xc3)&&(k<3);i++,k++)
				{
					lcd_command(i);
					lcd_data(0);
					IOCLR0 = led[k];
					delay_ms(250);
				}
				IOSET0 = led[0]|led[1]|led[2];
				lcd_command(0x01);
			}
		}
}
