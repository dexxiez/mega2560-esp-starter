#ifndef UART_H
#define UART_H

#include <stdint.h>

typedef enum {
    UART_USB = 0, /* USART0, pins 0/1, wired to CH340 */
    UART_ESP,     /* USART3, pins 14/15, wired to ESP8266 */
    UART_COUNT
} uart_port_t;

void uart_init(uart_port_t port, uint32_t baud);
void uart_putc(uart_port_t port, char c);
void uart_puts(uart_port_t port, const char *s);

/* Returns next received byte, or -1 if RX buffer empty. Non-blocking. */
int uart_getc(uart_port_t port);

#endif
