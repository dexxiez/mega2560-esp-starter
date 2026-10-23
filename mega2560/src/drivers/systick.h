#ifndef SYSTICK_H
#define SYSTICK_H

#include <stdint.h>

/* 1 ms tick on Timer0. Requires global interrupts enabled. */
void systick_init(void);
uint32_t systick_millis(void);

#endif
