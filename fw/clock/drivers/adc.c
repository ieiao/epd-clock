#include "adc.h"
#include <msp430g2553.h>

int16_t get_adc_value(void)
{
    uint16_t value;
    ADC_LINK_ON;
    ADC10CTL0 = SREF_1 + ADC10SHT_2 + REF2_5V + REFON + ADC10ON;
    delay_ms(10);
    ADC10CTL1 = INCH_0 + ADC10SSEL_2;
    ADC10AE0 |= 0x01;
    ADC10CTL0 |= ENC + ADC10SC;
    while(ADC10CTL1 & ADC10BUSY) {
        delay_ms(10);
    }
    value = ADC10MEM;
    ADC10CTL0 &= ~(ENC + ADC10SC);
    ADC10CTL0 &= ~(ADC10ON + REFON);
    ADC_LINK_OFF;

    return value;
}
