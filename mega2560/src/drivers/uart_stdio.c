#include "drivers/uart_stdio.h"

#include "drivers/uart.h"

#include <stdio.h>

static int stdio_putchar(char c, FILE *stream) {
    (void)stream;
    if (c == '\n')
        uart_putc(UART_USB, '\r');
    uart_putc(UART_USB, c);
    return 0;
}

static FILE usb_out = FDEV_SETUP_STREAM(stdio_putchar, NULL, _FDEV_SETUP_WRITE);

void uart_stdio_init(void) {
    stdout = &usb_out;
    stderr = &usb_out;
}
