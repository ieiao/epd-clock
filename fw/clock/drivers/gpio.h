#ifndef _GPIO_H_
#define _GPIO_H_

#include <msp430g2553.h>

#define EPD_PWR_CTL_PIN    BIT3
#define EPD_PWR_CTL_PORT   P3OUT
#define EPD_PWR_ON         (EPD_PWR_CTL_PORT &= ~EPD_PWR_CTL_PIN)
#define EPD_PWR_OFF        (EPD_PWR_CTL_PORT |= EPD_PWR_CTL_PIN)

#define EPD_BUSY_PIN       BIT1
#define EPD_BUSY_PORT      P3IN
#define EPD_BUSY_STATE     (EPD_BUSY_PORT & EPD_BUSY_PIN)

#define EPD_DC_PIN         BIT6
#define EPD_DC_PORT        P3OUT
#define EPD_DC_SET         (EPD_DC_PORT |= EPD_DC_PIN)
#define EPD_DC_RESET       (EPD_DC_PORT &= ~EPD_DC_PIN)

#define EPD_RESET_PIN      BIT7
#define EPD_RESET_PORT     P3OUT
#define EPD_RESET_SET      (EPD_RESET_PORT |= EPD_RESET_PIN)
#define EPD_RESET_RESET    (EPD_RESET_PORT &= ~EPD_RESET_PIN)

#define EPD_CS_PIN         BIT5
#define EPD_CS_PORT        P3OUT
#define EPD_CS_SET         (EPD_CS_PORT |= EPD_CS_PIN)
#define EPD_CS_RESET       (EPD_CS_PORT &= ~EPD_CS_PIN)

#define SPI_NOR_CS_PIN     BIT3
#define SPI_NOR_CS_PORT    P1OUT
#define SPI_NOR_CS_SET     (SPI_NOR_CS_PORT |= SPI_NOR_CS_PIN)
#define SPI_NOR_CS_RESET   (SPI_NOR_CS_PORT &= ~SPI_NOR_CS_PIN)

#define ADC_CTL_PIN        BIT4
#define ADC_CTL_PORT       P1OUT
#define ADC_LINK_ON        (ADC_CTL_PORT &= ~ADC_CTL_PIN)
#define ADC_LINK_OFF       (ADC_CTL_PORT = ADC_CTL_PIN)

#define BT0_PIN            BIT0
#define BT0_PORT           P2IN
#define BT0_STATE          (BT0_PORT & BT0_PIN)
#define BT0_PUSHED         (BT0_STATE == 0)

#define BT1_PIN            BIT1
#define BT1_PORT           P2IN
#define BT1_STATE          (BT1_PORT & BT1_PIN)
#define BT1_PUSHED         (BT1_STATE == 0)

#define BT2_PIN            BIT2
#define BT2_PORT           P2IN
#define BT2_STATE          (BT2_PORT & BT2_PIN)
#define BT2_PUSHED         (BT2_STATE == 0)

void gpio_init(void);
void gpio_suspend(void);
void gpio_resume(void);

#endif