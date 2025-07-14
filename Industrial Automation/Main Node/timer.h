volatile unsigned int second_counter = 0;
volatile unsigned char send_flag = 0;

void Timer1_ISR(void) __irq
{
    T1IR = 0x01; // Clear interrupt flag (MR0)
    second_counter++;

    if (second_counter >= 15)
    {
        send_flag = 1;
        second_counter = 0;
    }

    VICVectAddr = 0x00; // Acknowledge interrupt
}

void Timer1_Init(void)
{
    T1PR = 60000 - 1;   // Prescaler: 60MHz / 60000 = 1ms tick
    T1MR0 = 1000;       // 1000 x 1ms = 1s match
    T1MCR = 3;          // Interrupt & reset on MR0
    T1TCR = 1;          // Enable Timer1
}

void Timer1_Interrupt_Init(void)
{
    VICIntSelect &= ~(1 << 5);        // Timer1 interrupt is IRQ
    VICVectCntl2 = (1 << 5) | 5;      // Enable slot and assign Timer1 (interrupt #5)
    VICVectAddr2 = (unsigned long)Timer1_ISR; // ISR address
    VICIntEnable |= (1 << 5);         // Enable Timer1 interrupt
}
