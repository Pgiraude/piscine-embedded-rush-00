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

#define DEFAULT_BIT 0x00
#define PLAYER_READY_BIT 0x50
#define START_GAME_COUNTDOWN_BIT 0x51
#define OPPONENT_LOST_BIT 0x52
#define OPPONENT_BUTTON_PRESSED 0x53

#define WON 0
#define LOST 1

#define SW2_PRESSED !(PIND & (1 << PD4))

#define GAME_WAITING_PLAYER 0
#define GAME_COUNTDOWN 1
#define GAME_STARTED 2
#define GAME_OVER 3
#define GAME_REBOOT 4

uint8_t button_pressed;
uint8_t game_state;
uint8_t timer_game;
uint8_t is_master_ready;
uint8_t is_slave_ready;

void master_init();
void slave_init();
void master_loop();
void slave_loop();
void ft_error(uint8_t status);
void end_game(uint8_t result);
void interrupt_init();
void light(uint8_t color);
void set_game_start(void);
void init_game_timer(void);

#endif