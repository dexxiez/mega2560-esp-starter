#include "board.h"
#include "drivers/systick.h"
#include "drivers/uart.h"
#include "drivers/uart_stdio.h"

#include <avr/interrupt.h>
#include <stdio.h>

#define BLINK_MS 500

/* Forward bytes USB <-> ESP so AT commands can be typed from the monitor. */
static void bridge_poll(void) {
    int c;

    while ((c = uart_getc(UART_USB)) >= 0)
        uart_putc(UART_ESP, (char)c);
    while ((c = uart_getc(UART_ESP)) >= 0)
        uart_putc(UART_USB, (char)c);
}

int main(void) {
    LED_DDR |= _BV(LED_PIN);

    uart_init(UART_USB, USB_BAUD);
    uart_init(UART_ESP, ESP_BAUD);
    uart_stdio_init();
    systick_init();
    sei();

    printf("mega up. bridging USB <-> ESP8266\n");

    uint32_t last_blink = 0;
    for (;;) {
        uint32_t now = systick_millis();
        if (now - last_blink >= BLINK_MS) {
            last_blink = now;
            LED_PORT ^= _BV(LED_PIN);
        }
        bridge_poll();
    }
}
