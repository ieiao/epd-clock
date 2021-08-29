#ifndef _I2C_H_
#define _I2C_H_

#define SLAVE_ADDR 0x51

void i2c_init(void);
char i2c_master_write(char dev_addr, char reg_addr, char *tx_buffer, int length);
char i2c_master_read(char dev_addr, char reg_addr, char *rx_buffer, int length);

#endif