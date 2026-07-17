#include <stdbool.h>
#ifndef DRIVERS_UART_SERIAL_H
#define DRIVERS_UART_SERIAL_H

#define SERIAL_COM1 0x3F8

/* DEFAULT OUTPUT PORT FOR SERIAL COMMUNICATION
 * you can change this btw
 */
#define DEFAULT_PORT SERIAL_COM1

bool serial_init(void);
bool serial_signal_recived(void);
char serial_readch(void);
bool serial_transmit_empty(void);
void serial_putc(char c);
void serial_write(const char *str);

#endif /* DRIVERS_UART_SERIAL_H */