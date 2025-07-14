#define FREQ 15000000

unsigned int adc_value1 = 0;
float voltage1 = 0.0;


// UART1 Initialization for ESP-01 (TxD1 = P0.8, RxD1 = P0.9)
void UART1_Init() {
    PINSEL0 |= 0x00050000;  // Enable TxD1 and RxD1
    U1LCR = 0x83;           // Enable DLAB
    U1DLM = 1;
    U1DLL = 134;             // Baud rate = 9600 for 15MHz
    U1LCR = 0x03;           // 8-bit, no parity, 1 stop bit
}

void UART1_TxChar(char ch) {
    while (!(U1LSR & 0x20));
    U1THR = ch;
}

void UART1_SendString(const char *str) {
    while (*str) {
        UART1_TxChar(*str++);
    }
}

// Read ADC value from AD0.2 (P0.29)
int ADC_Conversion() {
    int value;
    ADCR |= 0x01000000;               // Start conversion
    while ((ADDR & 0x80000000) == 0); // Wait for conversion
    value = (ADDR >> 6) & 0x03FF;     // Extract 10-bit value
    return value;
}

// Display ADC and voltage on LCD
void Display_LCD(unsigned int adc, float volt) {
    LCD_COMMAND(0x80);
    LCD_String("ADC:");
    LCD_INTEGER(adc);
    LCD_COMMAND(0xC0);
    LCD_String("Volt:");
    LCD_FLOAT(volt);
}

// Send AT commands to connect ESP-01 to WiFi
void ESP_Init() {
    UART1_SendString("AT\r\n");
    delay_ms(2000);

    UART1_SendString("AT+CWMODE=1\r\n");  // Station mode
    delay_ms(2000);

    UART1_SendString("AT+CWJAP=\"jio 4G\",\"12345678\"\r\n"); // Your WiFi
    delay_ms(6000);
}

// Send ADC value to ThingSpeak field1
void ThingSpeak_Update(unsigned int value) {
    char buffer[200];
    char cmd[50];
    int len;

    // Start TCP connection
    UART1_SendString("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n");
    delay_ms(4000);

    // Correct HTTP GET format
    sprintf(buffer,
        "GET /update?api_key=9VSR9HCERI5UVLSM&field1=%d HTTP/1.1\r\nHost: api.thingspeak.com\r\nConnection: close\r\n\r\n",
        value);

    len = strlen(buffer);

    // Send length first
    sprintf(cmd, "AT+CIPSEND=%d\r\n", len);
    UART1_SendString(cmd);
    delay_ms(2000);

    // Send actual data
    UART1_SendString(buffer);
    delay_ms(4000);
}

int main() {
    PINSEL1 = 0x04000000;  // P0.29 as AD0.2
    LCD_INIT();            // Initialize LCD
    UART1_Init();          // Initialize UART1
    ESP_Init();            // Connect WiFi

    while (1) {
		ADCR = 0x00200D04;
        adc_value1 = ADC_Conversion();
        voltage1 = ((float)adc_value1 / 1023.0) * 3.3;

        Display_LCD(adc_value1, voltage1);     // Show on LCD
        ThingSpeak_Update(adc_value1);         // Send to cloud

        delay_ms(15000); // 15 seconds delay (ThingSpeak limit)
    }
}
