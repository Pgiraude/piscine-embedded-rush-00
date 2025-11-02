#include "TWI.h"

void slave_init() {
    TWAR = (SLAVE_ADDR << 1); // save the address in [7-1] bits, bit [0] is for
    TWCR = (1 << TWEN) | (1 << TWEA) | (1 << TWINT);
}

void slave_state_receive(void) {
    switch (game_state) {
    case GAME_WAITING_PLAYER:
        if (TWDR == START_GAME_COUNTDOWN_BIT) {
            button_pressed = 0;
            game_state = GAME_COUNTDOWN;
            set_game_start();
        }
        break;
    case GAME_COUNTDOWN:
        if (TWDR == OPPONENT_LOST_BIT) {
            button_pressed = 0;
            game_state = GAME_OVER;
        }
        break;
    case GAME_STARTED:
        break;
    case GAME_OVER:
    default:
        TWDR = DEFAULT_BIT;
    }
}

void slave_state_transmit(void) {
    switch (game_state) {
    case GAME_WAITING_PLAYER:
        if (button_pressed) {
            TWDR = PLAYER_READY_BIT;
            button_pressed = 0;
        }
        break;
    case GAME_COUNTDOWN:
        if (button_pressed) {
            TWDR = OPPONENT_LOST_BIT;
            button_pressed = 0;
        }
        break;
    case GAME_STARTED:
        if (button_pressed) {
            TWDR = OPPONENT_BUTTON_PRESSED;
            button_pressed = 0;
        }
        break;
    case GAME_OVER:
        break;
    }
}

void slave_loop(void) {
    uint8_t f_game_over = 0;

    init_game_timer();
    slave_init();
    interrupt_init();
    while (1) {
        if (TWCR & (1 << TWINT)) {
            uint8_t status = TWSR & TW_STATUS_MASK;

            switch (status) {
            case TW_ST_SLA_ACK:
            case TW_ST_DATA_ACK:

                slave_state_transmit();

            case TW_ST_DATA_NACK:
            case TW_SR_STOP:
            case TW_SR_SLA_ACK:
                break;

            case TW_SR_DATA_ACK:

                slave_state_receive();

                break;
            default:
                ft_error(ERROR_1);
            }
            TWCR = (1 << TWEN) | (1 << TWEA) | (1 << TWINT);
        }
    }
}
