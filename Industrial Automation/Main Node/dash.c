#include<lpc21xx.h>
#include<stdio.h>
#include<string.h>
#include "delay.h"
#include "types.h"
#include "lcd.h"
#include "can.h"
#include "i2c_defines.h"
#include "i2c.h"
#include "rtc.h"
#include "timer.h"
#include "peripherals.h"
#define buzr 1<<21
#define led1 1<<17
#define led2 1<<18

unsigned char sec = 0,min = 0,hours = 0,day = 0,date = 0,month = 0,year = 0;
volatile char emergency = 0,emergency_flag = 0;
int temp=0,moi=0, water = 0;                                       

void can_rx_intr() __irq
{
   	m1_rx.id=C2RID;
	m1_rx.dlc=(C2RFS>>16)&0xF;
	m1_rx.rtr=(C2RFS>>30)&0x1;
	if(m1_rx.rtr==0){ //if data frame
		m1_rx.byteA=C2RDA;
	}
	C2CMR=(1<<2);
	if(m1_rx.id == 0x001)
	{
		emergency = 1;		
	}
	else if(m1_rx.id == 0x002)
	{
		emergency = 0;
	}
	else if(m1_rx.id == 0x101)
	{
		moi = m1_rx.byteA;
	}

	else if(m1_rx.id == 0x102)
	{
		temp = m1_rx.byteA;
	}

	else if(m1_rx.id == 0x103)
	{
		water = m1_rx.byteA;
	}
	VICVectAddr = 0;
}

void ext_int1_isr()__irq

{
    EXTINT = 1 << 1;    // Clear EINT1 interrupt flag
    emergency_flag = 1;
    VICVectAddr = 0;     // Acknowledge interrupt

}


void display_status(void)
{
		rtc_read(&sec,&min,&hours,&day,&date,&month,&year);
		lcd_command(0x01);
		delay_ms(2);
		lcd_command(0x80);
		lcd_string("T:");
		lcd_num(hours);
		lcd_data(':');
		lcd_num(min);
		lcd_data(' ');
		lcd_string("W:");
		lcd_num(water);
		lcd_command(0xc0);
		lcd_string("Temp:");
		lcd_num(temp);
		lcd_command(0xc8);
		lcd_string("Moi:");
		lcd_num(moi);
		delay_ms(300);
		if (send_flag)
        {
            send_flag = 0;    // Clear flag
            ThingSpeak_Update(temp,moi,water);
        }
}


void intr_init()
{
		// Configure CAN interrupt (VIC channel 27)
		VICIntSelect &= ~(1 << 27); // IRQ for CAN
		VICVectCntl1 = 0x20 | 27;
		VICVectAddr1 = (unsigned long)can_rx_intr;
		VICIntEnable |= (1 << 27);
		C2IER = 0x01;

		PINSEL0 &= ~(3 << 28); // Clear bits 29:28
		PINSEL0 |=  (1 << 29); // Set bit 28 to 1, bit 29 to 0 -> EINT1
		EXTMODE  = 0x00;    // Level-sensitive
		EXTPOLAR = 0x00;    // Low level
		// IRQ for EINT1 (VIC channel 15)
		VICIntSelect &=  ~(1 << 15); // Select IRQ
		VICVectCntl0 = (0x20) | 15;
		VICVectAddr0 = (unsigned long)ext_int1_isr;
		VICIntEnable |=  (1 << 15); 
}


void configs()
{
		VPBDIV = 1;
		lcd_config();
		can2_init();
		init_i2c();
		Timer1_Init();
		Timer1_Interrupt_Init();
		UART1_Init();
		ESP_Init();
		IODIR0 |= buzr;
		IODIR0 |= led1|led2;
		IOSET0 = led1| led2;
		IOCLR0 = buzr;
		rtc_write(sec,min,hours,day,date,month,year);
}

void emr_flag_hand(void)
{
        delay_ms(100);          
		emergency = !emergency;
		if(emergency)
		{
			m1_tx.id = 0x001; // emergency message id
			can2_tx(m1_tx);
		}
		else
		{
			m1_tx.id = 0x002;// not an emergency message id
			can2_tx(m1_tx);
		}
		emergency_flag = 0;    // reset flag
}

void emergency_handler(void)
{
	    lcd_command(0x01);          // Clear LCD
		delay_ms(2);
	    lcd_command(0x80);          // Set cursor
	    lcd_string("Emergency");    // Display message
	
	    IOSET0 = buzr;              // Turn ON Buzzer
	    IOCLR0 = led2;              // Turn OFF LED
	    delay_ms(250);
	
	    lcd_command(0x01);          // Clear LCD again
		delay_ms(2);
	    IOCLR0 = buzr;              // Turn OFF Buzzer
	    IOSET0 = led2;              // Turn ON LED
	    delay_ms(250);
}



int main()
{
		configs();
		intr_init();
		lcd_command(0x80);
		lcd_string("Indus Automa");
		delay_ms(1000);
		lcd_command(0x01);
		delay_ms(2);
		m1_tx.id = 0x20;
		m1_tx.rtr = 0;
		m1_tx.dlc = 4;
		m1_tx.byteA = 0x00000000;

		while(1)
		{
			if (emergency_flag)
			{
				emr_flag_hand();
    		}
			if(!emergency)
			{
			   display_status();
			}
			if(emergency) 
			{
				emergency_handler();
			}
		}
}
