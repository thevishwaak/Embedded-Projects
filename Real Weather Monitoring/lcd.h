#ifndef DELAY_H
#define DELAY_H

void delay_s(unsigned int s);
void delay_ms(unsigned int s);
void delay_mics(unsigned int s);

#endif


#define lcd_d 0xf<<20
#define rs 1<<17
#define rw 1<<18
#define e 1<<19

void lcd_command(unsigned char cmd)
{
	IOCLR1 = lcd_d;
	IOSET1 = ((cmd & 0xF0) << 16);
	IOCLR1 = rs;
	IOCLR1 = rw;
	IOSET1 = e;
	delay_ms(2);
	IOCLR1 = e;

	IOCLR1 = lcd_d;
	IOSET1 = ((cmd & 0x0F) << 20);
	IOCLR1 = rs;
	IOCLR1 = rw;
	IOSET1 = e;
	delay_ms(2);
	IOCLR1 = e;
}

void lcd_data(unsigned char data)
{
	IOCLR1 = lcd_d;
	IOSET1 = ((data & 0xF0) << 16);
	IOSET1 = rs;
	IOCLR1 = rw;
	IOSET1 = e;
	delay_ms(2);
	IOCLR1 = e;

	IOCLR1 = lcd_d;
	IOSET1 = ((data & 0x0F) << 20);
	IOSET1 = rs;
	IOCLR1 = rw;
	IOSET1 = e;
	delay_ms(2);
	IOCLR1 = e;
}

void lcd_config()
{
	IODIR1 |= lcd_d | rs | rw | e;
	IOCLR1 = lcd_d | rs | rw | e;
	lcd_command(0x01);
	lcd_command(0x02);
	lcd_command(0x0c);
	lcd_command(0x28);
	lcd_command(0x80);
}

void lcd_string(char *str)
{
	while(*str)
	{
		lcd_data(*str++);
	}
}

void lcd_time_display(u8 t)
{
	//lcd_command(l);
	lcd_data((t/10)+'0');
	lcd_data((t%10)+'0');
}

