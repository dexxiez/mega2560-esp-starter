#ifndef BOARD_H
#define BOARD_H

#include <avr/io.h>

/* On-board LED "L" = Arduino pin 13 = PB7 */
#define LED_DDR  DDRB
#define LED_PORT PORTB
#define LED_PIN  PB7

#define USB_BAUD 115200UL /* USART0 -> CH340 */
#define ESP_BAUD 115200UL /* USART3 -> ESP8266 (DIP 1+2 ON), stock AT fw default */

#endif
