#ifndef _UART_H_
#define _UART_H_

void uart_init(void);
void uart_write(char *p);
char uart_getc(void);
char uart_putc(char c);

#endif