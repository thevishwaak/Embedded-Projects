#define trig (1 << 10)  // P0.10
#define echo (1 << 11)  // P0.11

void ultrasonic_init()
{
	IODIR0 |= trig;
	T0TCR = 0;
	T0PR = 60-1;
}

void send_pulse()
{
	T0TC = T0PC = 0;
	IOSET0 = trig;
	delay_us(10);
	IOCLR0 = trig;
}
float get_range()
{
	float time = 0,distance = 0;
	send_pulse();
	while(!(IOPIN0 & echo));
	T0TCR = 0x01;
	while(IOPIN0 & echo);
	T0TCR =0x00;
	time = T0TC;
	if(time < 38000)
	{
		distance = time / 59.0;
	}
	else
	{
		distance =0;
	}
	return distance;
}

unsigned int get_percentage(unsigned int tds)
{
	unsigned value = 0;
	float distance = 0;
	distance = get_range();
	value = (distance / (float)tds) * 100;
	if(value > 100)
	{
		return 0;
	}
	else if(value <= 0)
	{
		return 100;
	}
	else
	{
		return 100 - value;
	}
}

