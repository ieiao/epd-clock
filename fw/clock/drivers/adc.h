#ifndef __ADC_H__
#define __ADC_H__

#include <msp430g2553.h>
#include <stdint.h>
#include "timer.h"
#include "gpio.h"

int16_t get_adc_value(void);

#endif
