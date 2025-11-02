#include "TWI.h"

void slave_init() {
    TWAR = (SLAVE_ADDR << 1); // save the address in [7-1] bits, bit [0] is for
	TWCR = (1 << TWEN) | (1 << TWEA) | (1 << TWINT);
}

void slave_loop(void) {
    uint8_t screen_score = 1;
    screen_countdown = 0;
    screen_playing = 0;
    uint8_t f_game_over = 0;
    uint8_t slave_ready = 1;
    uint8_t data;

    interrupt_init();
    while (1) {
        if (TWCR & (1 << TWINT)) {
            uint8_t status = TWSR & TW_STATUS_MASK;

            switch (status) {
            case TW_ST_SLA_ACK:
            case TW_ST_DATA_ACK:
                if (button_pressed == 1 && screen_playing == 1) {
                    TWDR = OPPONENT_BUTTON_PRESSED;
                    light(GREEN);
                    screen_score = 1;
                    screen_playing = 0;
                    button_pressed = 0;
                } else if (button_pressed == 1 && screen_countdown == 1) {
                    TWDR = OPPONENT_LOST;
                    stop_timer();
                    light(RED);
                    screen_score = 1;
                    screen_countdown = 0;
                    button_pressed = 0;
                } else if (button_pressed == 1 && screen_score == 1 && slave_ready == 0) {
                    TWDR = OPPONENT_BUTTON_PRESSED;
                    light(LED1);
                    slave_ready = 1;
                    button_pressed = 0;
                } else
                    TWDR = 'A';
                break;

            case TW_ST_DATA_NACK:
            case TW_SR_STOP:
            case TW_SR_SLA_ACK:
                break;

            case TW_SR_DATA_ACK:
                data = TWDR;
                if (data == OPPONENT_BUTTON_PRESSED) {
                    light(RED);
                    screen_score = 1;
                    screen_playing = 0;
                    slave_ready = 0;
                }
            
                else if (data == TIMER_NOW) {
                    light(BLACK);
                    call_timer();
                    screen_countdown = 1;
                    screen_score = 0;
                    slave_ready = 0;
                }
                else if (data == OPPONENT_LOST) {
                    light(GREEN);
                    stop_timer();
                    screen_countdown = 0;
                    screen_score = 1;
                    slave_ready = 0;
                }
                break;
            default:
                ft_error(ERROR_1);
            }
            TWCR = (1 << TWEN) | (1 << TWEA) | (1 << TWINT);
        }
    }
}
