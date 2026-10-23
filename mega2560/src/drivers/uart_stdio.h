#ifndef UART_STDIO_H
#define UART_STDIO_H

/* Route stdout (printf) to the USB UART. Call after uart_init(UART_USB, ...). */
void uart_stdio_init(void);

#endif
