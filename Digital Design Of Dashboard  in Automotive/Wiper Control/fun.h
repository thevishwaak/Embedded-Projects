#include <lpc21xx.h>
char angle;
void pwm_config(void)
{
    PINSEL0 |= 0x00000002;      // Configure P0.0 as PWM1 output
    PWMPR = 59;                 // Prescaler to make each PWMTC increment 1 µs
    PWMMR0 = 20000;             // Set PWM period to 20 ms
    PWMMCR = (1 << 1);          // Reset PWMTC on PWMMR0 match
    PWMPCR = (1 << 9);          // Enable PWM1 output
    PWMLER = (1 << 0);          // Latch enable for PWMMR0
    PWMTCR = (1 << 0) | (1 << 3); // Enable counter and PWM mode
}

void set_servo_position(char angle)
{
    PWMMR1 = 500 + ((angle * 2000) / 180);      
    PWMLER = (1 << 1);          // Latch enable for PWMMR1
}

void wiper_slow(void)
{
    for (angle = 0; angle <= 180; angle++) {
        set_servo_position(angle);
        delay_ms(7);
    }
    for (angle = 180; angle > 0; angle--) {
        set_servo_position(angle);
        delay_ms(7);
    }
}

void wiper_mid(void)
{
    for (angle = 0; angle <= 180; angle++) {
        set_servo_position(angle);
        delay_ms(5);
    }
    for (angle = 180; angle > 0; angle--) {
        set_servo_position(angle);
        delay_ms(5);
    }
}

void wiper_fast(void)
{
    for (angle = 0; angle <= 180; angle++) {
        set_servo_position(angle);
        delay_ms(3);
    }
    for (angle = 180; angle > 0; angle--) {
        set_servo_position(angle);
        delay_ms(3);
    }
}

