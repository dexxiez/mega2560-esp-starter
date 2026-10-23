#ifndef HEARTBEAT_H
#define HEARTBEAT_H

#include <stdint.h>

/* Spawn a FreeRTOS task that logs uptime + free heap every period_ms. */
void heartbeat_start(uint32_t period_ms);

#endif
