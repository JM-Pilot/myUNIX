#include <stdbool.h>
#ifndef DRIVERS_UART_SERIAL_H
#define DRIVERS_UART_SERIAL_H

#define SERIAL_COM1 0x3F8
#define SERIAL_COM2 0x2F8
#define SERIAL_COM3 0x3E8
#define SERIAL_COM4 0x2E8
#define SERIAL_COM5 0x5F8
#define SERIAL_COM6 0x4F8
#define SERIAL_COM7 0x5E8
#define SERIAL_COM8 0x4E8

bool serial_init(void);
bool serial_signal_recived(void);
char serial_readch(void);
bool serial_transmit_empty(void);
void serial_putc(char c);
void serial_write(const char *str);
void serial_switch(uint32_t port);

#endif /* DRIVERS_UART_SERIAL_H */
