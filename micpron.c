#include <16f877a.h>
#device ADC=10
#fuses NOWDT, HS
#use delay(clock=20MHz)

#define LCD_DATA4       PIN_D4 
#define LCD_DATA5       PIN_D5 
#define LCD_DATA6       PIN_D6 
#define LCD_DATA7       PIN_D7 
#define LCD_ENABLE_PIN  PIN_C2 
#define LCD_RW_PIN      PIN_C1 
#define LCD_RS_PIN      PIN_C0 

#include <lcd.c>

unsigned int16 ohmVal = 0, currVal = 0, voltVal = 0;
float ohm = 0, current = 0, voltage = 0;
float oldOhm = -1, oldCurr = -1, oldVolt = -1; // Initialize to -1 to ensure first update

void main() {
    lcd_init();
    setup_adc_ports(ALL_ANALOG);
    setup_adc(ADC_CLOCK_DIV_32);
    printf(lcd_putc, "\f"); // Clear LCD

    while(TRUE) {
        // Read ohm value
        set_adc_channel(3);
        delay_us(20);
        ohmVal = read_adc();
        float ohmVoltage = (float)ohmVal * (5.0 / 1023.0);
        ohm = (ohmVoltage * 10000) / (5.0 - ohmVoltage);

        // Read current value
        set_adc_channel(2);
        delay_us(20);
        currVal = read_adc();
        float curV = (float)currVal * (5.0 / 1023.0);
        current = (curV - 2.5) / 0.185;

        // Read voltage value
        set_adc_channel(1);
        delay_us(20);
        voltVal = read_adc();
        voltage = (float)voltVal * (5.0 * 10.0) / 1023.0;

        // Update LCD display if values have changed
        if (ohm != oldOhm) {
            lcd_gotoxy(1, 1);
            printf(lcd_putc, "\fOhm: %.2f   ", ohm);
            oldOhm = ohm; // Update old value
        }

        if (current != oldCurr) {
            lcd_gotoxy(1, 1);
            printf(lcd_putc, "\fCurr: %.2fA", current);
            oldCurr = current; // Update old value
        }

        if (voltage != oldVolt) {
            lcd_gotoxy(1, 1);
            printf(lcd_putc, "\fVolt: %.2fV", voltage);
            oldVolt = voltage; // Update old value
        }

        delay_ms(100); // Add delay to reduce flicker
    }
}

