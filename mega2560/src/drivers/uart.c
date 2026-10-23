#include "drivers/uart.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/atomic.h>

#define RX_BUF_SIZE 128 /* power of two */
#define RX_MASK (RX_BUF_SIZE - 1)

typedef struct {
    volatile uint8_t *ucsra;
    volatile uint8_t *ucsrb;
    volatile uint8_t *ucsrc;
    volatile uint16_t *ubrr;
    volatile uint8_t *udr;
} uart_regs_t;

typedef struct {
    volatile uint8_t buf[RX_BUF_SIZE];
    volatile uint8_t head;
    volatile uint8_t tail;
} rx_ring_t;

/* USART register bit positions are identical across USART0..3 on the 2560. */
static const uart_regs_t regs[UART_COUNT] = {
    [UART_USB] = {&UCSR0A, &UCSR0B, &UCSR0C, &UBRR0, &UDR0},
    [UART_ESP] = {&UCSR3A, &UCSR3B, &UCSR3C, &UBRR3, &UDR3},
};

static rx_ring_t rx[UART_COUNT];

void uart_init(uart_port_t port, uint32_t baud) {
    const uart_regs_t *r = &regs[port];

    /* Double-speed mode: lower baud error at 115200 on a 16 MHz crystal. */
    *r->ucsra = _BV(U2X0);
    *r->ubrr = (uint16_t)((F_CPU + baud * 4) / (baud * 8) - 1);
    *r->ucsrc = _BV(UCSZ01) | _BV(UCSZ00); /* 8N1 */
    *r->ucsrb = _BV(RXEN0) | _BV(TXEN0) | _BV(RXCIE0);
}

void uart_putc(uart_port_t port, char c) {
    const uart_regs_t *r = &regs[port];

    while (!(*r->ucsra & _BV(UDRE0)))
        ;
    *r->udr = (uint8_t)c;
}

void uart_puts(uart_port_t port, const char *s) {
    while (*s)
        uart_putc(port, *s++);
}

int uart_getc(uart_port_t port) {
    rx_ring_t *q = &rx[port];
    int c = -1;

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        if (q->head != q->tail) {
            c = q->buf[q->tail];
            q->tail = (q->tail + 1) & RX_MASK;
        }
    }
    return c;
}

static inline void rx_push(uart_port_t port, uint8_t byte) {
    rx_ring_t *q = &rx[port];
    uint8_t next = (q->head + 1) & RX_MASK;

    if (next != q->tail) { /* drop byte on overflow */
        q->buf[q->head] = byte;
        q->head = next;
    }
}

ISR(USART0_RX_vect) {
    rx_push(UART_USB, UDR0);
}
ISR(USART3_RX_vect) {
    rx_push(UART_ESP, UDR3);
}
