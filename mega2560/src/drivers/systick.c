#include "drivers/systick.h"

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/atomic.h>

static volatile uint32_t ticks;

void systick_init(void)
{
    /* CTC mode, prescaler 64: 16 MHz / 64 / 250 = 1 kHz */
    TCCR0A = _BV(WGM01);
    TCCR0B = _BV(CS01) | _BV(CS00);
    OCR0A  = 249;
    TIMSK0 = _BV(OCIE0A);
}

uint32_t systick_millis(void)
{
    uint32_t t;

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        t = ticks;
    }
    return t;
}

ISR(TIMER0_COMPA_vect) { ticks++; }
