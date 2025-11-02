#ifndef TWI_H
#define TWI_H

#include <avr/io.h>
#include <util/delay.h>
#include <util/twi.h>
#include <avr/interrupt.h>

#define WRITE		0
#define READ		1
#define SLAVE_ADDR	0x12
#define MASTER_ADDR	0x00

#define ERROR_1		1
#define ERROR_2		2
#define ERROR_3		3
#define ERROR_4		4
#define ERROR_5		5
#define ERROR_6		6
#define ERROR_7		7

#define WON 0
#define LOST 1

#define OPPONENT_BUTTON_PRESSED 1

uint8_t button_pressed;

void TWI_init(uint8_t my_address);
void ft_error(uint8_t status);
void end_game(uint8_t result);
void ready_flash();
void interrupt_init();

#endif