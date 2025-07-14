void delay_s(unsigned int s)
{
    T0TCR = 0x02;       // Reset timer
    T0TCR = 0x01;       // Start timer
    T0PR = 15000000 - 1; // Prescale for 1 second (assuming 15 MHz PCLK)
    T0TC = 0;           // Reset timer count

    while(T0TC < s);    // Wait for s seconds

    T0TCR = 0x00;       // Stop timer
}

void delay_ms(unsigned int ms)
{
    T0TCR = 0x02;       // Reset timer
    T0TCR = 0x01;       // Start timer
    T0PR = 15000 - 1;    // Prescale for 1 ms (assuming 15 MHz PCLK)
    T0TC = 0;           // Reset timer count

    while(T0TC < ms);   // Wait for ms milliseconds

    T0TCR = 0x00;       // Stop timer
}

void delay_mics(unsigned int us)
{
    T0TCR = 0x02;       // Reset timer
    T0TCR = 0x01;       // Start timer
    T0PR = 15 - 1;       // Prescale for 1 microsecond (assuming 15 MHz PCLK)
    T0TC = 0;           // Reset timer count

    while(T0TC < us);   // Wait for us microseconds

    T0TCR = 0x00;       // Stop timer
}
