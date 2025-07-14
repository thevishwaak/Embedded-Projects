#define lcd_d 0xf<<20
#define rs 1<<17
#define rw 1<<18
#define e 1<<19

void delay_ms(unsigned int s)
{
	T0PR = 60000-1;
	T0TCR = 0x1;
	while(T0TC<s);
	T0TCR = 0x3;
	T0TCR = 0x0;
}
void lcd_command(unsigned char cmd)
{
	IOCLR1 = lcd_d;
	IOSET1 = ( 0xf0 & cmd )<<16;
	IOCLR1 = rs;
	IOCLR1 = rw;
	IOSET1 = e;
	delay_ms(2);
	IOCLR1 = e;
	
	IOCLR1 = lcd_d;
	IOSET1 = ( 0x0f & cmd )<<20;
	IOCLR1 = rs;
	IOCLR1 = rw;
	IOSET1 = e;
	delay_ms(2);
	IOCLR1 = e;	
}

void lcd_data(unsigned char data)
{
	IOCLR1 = lcd_d;
	IOSET1 = ( 0xf0 & data )<<16;
	IOSET1 = rs;
	IOCLR1 = rw;
	IOSET1 = e;
	delay_ms(2);
	IOCLR1 = e;
	
	IOCLR1 = lcd_d;
	IOSET1 = ( 0x0f & data )<<20;
	IOSET1 = rs;
	IOCLR1 = rw;
	IOSET1 = e;
	delay_ms(2);
	IOCLR1 = e;	

}

void lcd_config(void)
{
	IODIR1 |= lcd_d | rs | rw | e;
	IOCLR1 = lcd_d | rs | rw | e;
	lcd_command(0x01);
	lcd_command(0x02);
	lcd_command(0x0c);
	lcd_command(0x28);
	lcd_command(0x80);
}

void lcd_string(char *s)
{
	while(*s)
	{
		lcd_data(*s++);
	}
}
