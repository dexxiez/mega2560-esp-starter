#ifndef SHARED_EXAMPLE_H
#define SHARED_EXAMPLE_H

/*
 * Headers in shared/include/ are visible to both chips.
 * Keep them portable: plain C, <stdint.h>/<stdbool.h> only, no AVR or SDK headers.
 * Fixed-width types matter: int is 16-bit on AVR, 32-bit on ESP8266.
 */

#include <stdint.h>

#define SHARED_EXAMPLE_VERSION 1

#endif
