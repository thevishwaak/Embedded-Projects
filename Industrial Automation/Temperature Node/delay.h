// Delay in seconds
void delay_s(unsigned int s)
{
    T0TCR = 0x02;            // Reset timer
    T0PR = 60000000 - 1;     // 1-second tick at 60MHz PCLK
    T0TC = 0;                // Clear Timer Counter
    T0TCR = 0x01;            // Enable timer
    while(T0TC < s);         // Wait s seconds
    T0TCR = 0x00;            // Stop timer
}

// Delay in milliseconds
void delay_ms(unsigned int ms)
{
    T0TCR = 0x02;           // Reset timer
    T0PR = 60000 - 1;       // 1 ms tick at 60 MHz PCLK
    T0TC = 0;               // Clear counter
    T0TCR = 0x01;           // Start timer
    while(T0TC < ms);       // Wait ms milliseconds
    T0TCR = 0x00;           // Stop timer
}

// Delay in microseconds
void delay_us(unsigned int us)
{
    T0TCR = 0x02;           // Reset timer
    T0PR = 60 - 1;          // 1 us tick at 15 MHz PCLK
    T0TC = 0;               // Clear counter
    T0TCR = 0x01;           // Start timer
    while(T0TC < us);       // Wait us microseconds
    T0TCR = 0x00;           // Stop timer
}
