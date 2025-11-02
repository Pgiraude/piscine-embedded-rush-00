#ifndef TWI_H
#define TWI_H

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include <util/twi.h>

#define WRITE 0
#define READ 1
#define SLAVE_ADDR 0x12
#define MASTER_ADDR 0x00
#define MASTER 0
#define SLAVE 1
#define ERROR 2
#define FREE 0
#define BUSY 1

#define TIMER_NOW 5

#define RED 1
#define GREEN 2
#define BLUE 3
#define WHITE 4
#define BLACK 5
#define LED1 7
#define LED2 8
#define LED3 10
#define LED4 11

#define ERROR_1 1
#define ERROR_2 2
#define ERROR_3 3
#define ERROR_4 4
#define ERROR_5 5
#define ERROR_6 6
#define ERROR_7 7

#define WON 0
#define LOST 1

#define OPPONENT_BUTTON_PRESSED 1
#define OPPONENT_LOST 2

#define SW2_PRESSED !(PIND & (1 << PD4))

uint8_t button_pressed;
uint8_t playing;

void master_init();
void master_loop();
void slave_init();
void slave_loop();
void ft_error(uint8_t status);
void end_game(uint8_t result);
void ready_flash();
void interrupt_init();
void light(uint8_t color);
void call_timer();

#endif