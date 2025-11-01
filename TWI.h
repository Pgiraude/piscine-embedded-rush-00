#ifndef TWI_H
#define TWI_H

#include <avr/io.h>
#include <util/delay.h>
#include <util/twi.h>
#include <avr/interrupt.h>

#define WRITE		0
#define READ		1
#define SLAVE_ADDR	0x20
#define MASTER_ADDR	0x00

#define SUCCESS		0
#define ERROR_1		1
#define ERROR_2		2
#define ERROR_3		3
#define ERROR_4		4
#define ERROR_5		5
#define ERROR_6		6
#define ERROR_7		7
#define ERROR_8		8
#define ERROR_9		9
#define ERROR_10	10

#define SLAVE_RECIVE 1
#define SLAVE_TRANSMIT 2
#define TRANSMIT 1
#define RECIVE 2

#define SLAVE_BUTTON_PRESSED 1
#define MASTER_BUTTON_PRESSED 2
#define SLAVE_WON 3
#define MASTER_WON 4

#define SLAVE_LISTEN_START	0x60
#define SLAVE_LISTEN_STOP	0xA0

#define TWCR_SLAVE_RESET TWCR = (1 << TWEN) | (1 << TWEA) | (1 << TWINT)
#define TWCR_SEND_START TWCR = (1 << TWEN) | (1 << TWSTA) | (1 << TWINT)

#define LED_SLAVE_ON DDRB |= (1 << PB2);PORTB |= (1 << PB2)
#define LED_MASTER_ON DDRB |= (1 << PB4);PORTB |= (1 << PB4)

//uint8_t button_pressed = 0;

void TWI_init(uint8_t my_address);
void ft_error(uint8_t status);
void got_hit();

#endif