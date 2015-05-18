#ifndef __UART_H__
#define __UART_H__

void set_uart_timeout(int read_ms, int write_ms);
uint32 init_uart(int baudrate);
int uart_putc(int c);
int uart_getc(void);
char *uart_gets(char *str,int len);
int uart_puts(char *str);
int test(volatile char *str);

#endif
