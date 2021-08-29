#include <msp430g2553.h>

void platform_init(void)
{
    WDTCTL = WDTPW + WDTHOLD; // Stop WDT
    if (CALBC1_8MHZ == 0xFF)  // If calibration constant erased
    {
        while (1)
            ; // do not load, trap CPU!!
    }
    DCOCTL = 0;             // Select lowest DCOx and MODx settings
    BCSCTL1 = CALBC1_8MHZ; // Set DCO
    DCOCTL = CALDCO_8MHZ;
}

void int_enable(void)
{
    __bis_SR_register(GIE);
}

void int_disable(void)
{
    __bic_SR_register(GIE);
}
