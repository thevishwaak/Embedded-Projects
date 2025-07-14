#define lcd_d 0xf<<20
#define rs 1<<17
#define rw 1<<18
#define e 1<<19

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
	delay_ms(2);
}


void cgram_write(char nBytes)
{
	int i;
	lcd_command(0x40);
	for(i=0;i<nBytes;i++)
	{

	}
	lcd_command(0x80);		
}

void lcd_string(char *s)
{
	while(*s)
	{
		lcd_data(*s++);
	}
}

void lcd_time_display(u8 t)
{
	//lcd_command(l);
	lcd_data((t/10)+'0');
	lcd_data((t%10)+'0');
}

void lcd_num(unsigned int num)
{
    char buf[10];  // Buffer to hold the converted number
    int i = 0;

    // Handle zero explicitly
    if (num == 0) {
        lcd_data('0');
        return;
    }

    // Convert number to string (in reverse)
    while (num > 0) {
        buf[i++] = (num % 10) + '0';
        num = num / 10;
    }

    // Print the digits in correct order
    while (i--) {
        lcd_data(buf[i]);
    }
}
