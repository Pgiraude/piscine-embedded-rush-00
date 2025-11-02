#include "TWI.h"

void slave_init() {
    TWAR = (SLAVE_ADDR << 1); // save the address in [7-1] bits, bit [0] is for
	TWCR = (1 << TWEN) | (1 << TWEA) | (1 << TWINT);
}

void slave_loop(void) {
    uint8_t f_game_over = 0;

    interrupt_init();
    while (1) {
        if (TWCR & (1 << TWINT)) {
            if (f_game_over == 1)
                end_game(WON);
            uint8_t status = TWSR & TW_STATUS_MASK;

            switch (status) {
            case TW_ST_SLA_ACK:
            case TW_ST_DATA_ACK:
                if (button_pressed == 1) {
                    TWDR = OPPONENT_BUTTON_PRESSED;
                    f_game_over = 1;
                } else
                    TWDR = 'A';
                break;

            case TW_ST_DATA_NACK:
            case TW_SR_STOP:
            case TW_SR_SLA_ACK:
                break;

            case TW_SR_DATA_ACK:
                if (TWDR == OPPONENT_BUTTON_PRESSED)
                    end_game(LOST);
                break;
            default:
                ft_error(ERROR_1);
            }
            TWCR = (1 << TWEN) | (1 << TWEA) | (1 << TWINT);
        }
    }
}
