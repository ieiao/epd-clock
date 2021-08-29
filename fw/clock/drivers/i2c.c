#include <msp430g2553.h>
#include <stdint.h>
#include "i2c.h"
#include "uart.h"

enum i2c_mode
{
    IDLE_MODE,
    NACK_MODE,
    TX_REG_ADDRESS_MODE,
    RX_REG_ADDRESS_MODE,
    TX_DATA_MODE,
    RX_DATA_MODE,
    SWITCH_TO_RX_MODE,
    SWITHC_TO_TX_MODE,
    TIMEOUT_MODE
};

static volatile char master_mode;
static volatile char transmit_reg_addr;
static volatile char tx_bytes;
static volatile char rx_bytes;
static volatile char *tx_index;
static volatile char *rx_index;

void i2c_init(void)
{
    UCB0CTL1 |= UCSWRST;                  // Enable SW reset
    UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC; // I2C Master, synchronous mode
    UCB0CTL1 = UCSSEL_2 + UCSWRST;        // Use SMCLK, keep SW reset
    UCB0BR0 = 20;                         // fSCL = SMCLK/20 = ~400kHz
    UCB0BR1 = 0;
    UCB0I2CSA = SLAVE_ADDR; // Slave Address
    UCB0CTL1 &= ~UCSWRST;   // Clear SW reset, resume operation
    UCB0I2CIE |= UCNACKIE;
}

char i2c_master_write(char dev_addr, char reg_addr, char *tx_buffer, int length)
{
    master_mode = TX_REG_ADDRESS_MODE;
    transmit_reg_addr = reg_addr;
    tx_bytes = length;
    rx_bytes = 0;
    tx_index = tx_buffer;

    UCB0I2CSA = dev_addr;
    IFG2 &= ~(UCB0TXIFG + UCB0RXIFG); // Clear any pending interrupts
    IE2 &= ~UCB0RXIE;                 // Disable RX interrupt
    IE2 |= UCB0TXIE;                  // Enable TX interrupt

    UCB0CTL1 |= UCTR + UCTXSTT; // I2C TX, start condition
    __bis_SR_register(CPUOFF);  // Enter LPM0

    return master_mode;
}

char i2c_master_read(char dev_addr, char reg_addr, char *rx_buffer, int length)
{
    master_mode = TX_REG_ADDRESS_MODE;
    transmit_reg_addr = reg_addr;
    tx_bytes = 0;
    rx_bytes = length;
    rx_index = rx_buffer;

    UCB0I2CSA = dev_addr;
    IFG2 &= ~(UCB0TXIFG + UCB0RXIFG); // Clear any pending interrupts
    IE2 &= ~UCB0RXIE;                 // Disable RX interrupt
    IE2 |= UCB0TXIE;                  // Enable TX interrupt

    UCB0CTL1 |= UCTR + UCTXSTT; // I2C TX, start condition
    __bis_SR_register(CPUOFF);  // Enter LPM0

    return master_mode;
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = USCIAB0TX_VECTOR
__interrupt void USCIAB0TX_ISR(void)
#elif defined(__GNUC__)
void __attribute__((interrupt(USCIAB0TX_VECTOR))) USCIAB0TX_ISR(void)
#else
#error Compiler not supported!
#endif
{
    if (IFG2 & UCB0RXIFG) // Receive Data Interrupt
    {
        char tmp = UCB0RXBUF;
        if (rx_bytes > 0)
        {
            *rx_index = tmp;
            rx_index++;
            rx_bytes--;
        }

        if (rx_bytes == 1)
        {
            UCB0CTL1 |= UCTXSTP;
        }
        else if (rx_bytes == 0)
        {
            IE2 &= ~UCB0RXIE;
            master_mode = IDLE_MODE;
            __bic_SR_register_on_exit(CPUOFF); // Exit LPM0
        }
    }
    else if (IFG2 & UCB0TXIFG) // Transmit Data Interrupt
    {
        switch (master_mode)
        {
        case TX_REG_ADDRESS_MODE:
            UCB0TXBUF = transmit_reg_addr;
            if (rx_bytes)
                master_mode = SWITCH_TO_RX_MODE; // Need to start receiving now
            else
                master_mode = TX_DATA_MODE; // Continue to transmision with the data in Transmit Buffer
            break;

        case SWITCH_TO_RX_MODE:
            IE2 |= UCB0RXIE;              // Enable RX interrupt
            IE2 &= ~UCB0TXIE;             // Disable TX interrupt
            UCB0CTL1 &= ~UCTR;            // Switch to receiver
            if (rx_bytes == 1)
            {
                UCB0CTL1 |= UCTXSTT;          // Send repeated start
                //Must send stop since this is the N-1 byte
                while((UCB0CTL1 & UCTXSTT));
                UCB0CTL1 |= UCTXSTP;      // Send stop condition
            } else
                UCB0CTL1 |= UCTXSTT;          // Send repeated start
            break;

        case TX_DATA_MODE:
            if (tx_bytes)
            {
                UCB0TXBUF = *tx_index++;
                tx_bytes--;
            }
            else
            {
                //Done with transmission
                UCB0CTL1 |= UCTXSTP; // Send stop condition
                master_mode = IDLE_MODE;
                IE2 &= ~UCB0TXIE;                  // disable TX interrupt
                __bic_SR_register_on_exit(CPUOFF); // Exit LPM0
            }
            break;

        default:
            __no_operation();
            break;
        }
    }
}