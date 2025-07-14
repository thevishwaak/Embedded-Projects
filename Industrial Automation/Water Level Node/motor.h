void MOTOR_INIT(void)
{
	IODIR0 |= MOTOR_H|MOTOR_L;
	IOSET0 |= MOTOR_H|MOTOR_L;
}

void MOTOR_FORWARD(void)
{
	IOCLR0 = MOTOR_H;
	IOSET0 = MOTOR_L;	
}

void MOTOR_BACKWARD(void)
{
	IOCLR0 = MOTOR_L;
	IOSET0 = MOTOR_H;
}

void MOTOR_OFF(void)
{
	IOSET0 = MOTOR_H | MOTOR_L;
}
