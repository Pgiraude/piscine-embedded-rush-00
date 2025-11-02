#include "TWI.h"

void master_init() {
    TWBR = 72;
    TWSR = 0x00;
    TWCR = (1 << TWEN) | (1 << TWINT);
}

void master_state_transmit(void) {
    switch (game_state) {
    case GAME_WAITING_PLAYER:
        if (button_pressed) {
            is_master_ready = 1;
        }
        if (is_master_ready && is_slave_ready) {
            TWDR = START_GAME_COUNTDOWN_BIT;
            is_master_ready = 0;
            is_slave_ready = 0;
            game_state = GAME_COUNTDOWN;
            set_game_start();
        }
        break;
    case GAME_COUNTDOWN:
        if (button_pressed) {
            TWDR = OPPONENT_LOST_BIT;
            game_state = GAME_OVER;
        }
        break;
    case GAME_STARTED:
        if (button_pressed) {
            TWDR = OPPONENT_BUTTON_PRESSED;
            game_state = GAME_OVER;
        }
        break;
    case GAME_OVER:
        break;
    default:
        TWDR = DEFAULT_BIT;
    }
    button_pressed = 0;
}

void master_state_receive(void) {
    switch (game_state) {
    case GAME_WAITING_PLAYER:
        if (TWDR == PLAYER_READY_BIT) {
            is_slave_ready = 1;
        }
        break;
    case GAME_COUNTDOWN:
        if (TWDR == OPPONENT_LOST_BIT) {
            game_state = GAME_OVER;
        }
        break;
    case GAME_STARTED:
        if (TWDR == OPPONENT_BUTTON_PRESSED) {
            game_state = GAME_OVER;
        }
        break;
    default:
        TWDR = DEFAULT_BIT;
    }
}

void master_loop() {
    uint8_t f_game_over = 0;
    is_master_ready = 0;
    is_slave_ready = 0;

    interrupt_init();
    while (1) {
        //	TRANSMITING
        TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);

        while (!(TWCR & (1 << TWINT)))
            ;

        if ((TWSR & TW_STATUS_MASK) != TW_START)
            ft_error(ERROR_1);

        TWDR = (SLAVE_ADDR << 1) | WRITE;
        TWCR = (1 << TWINT) | (1 << TWEN);

        while (!(TWCR & (1 << TWINT)))
            ;

        if ((TWSR & TW_STATUS_MASK) != TW_MT_SLA_ACK)
            ft_error(ERROR_2);

        master_state_transmit();

        TWCR = (1 << TWINT) | (1 << TWEN);

        while (!(TWCR & (1 << TWINT)))
            ;

        if ((TWSR & TW_STATUS_MASK) != TW_MT_DATA_ACK)
            ft_error(ERROR_3);

        TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);

        _delay_ms(5);

        //	RECIVING
        TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);

        while (!(TWCR & (1 << TWINT)))
            ;

        if ((TWSR & TW_STATUS_MASK) != TW_START)
            ft_error(ERROR_4);

        TWDR = (SLAVE_ADDR << 1) | READ;
        TWCR = (1 << TWINT) | (1 << TWEN);

        while (!(TWCR & (1 << TWINT)))
            ;

        if ((TWSR & TW_STATUS_MASK) != TW_MR_SLA_ACK)
            ft_error(ERROR_5);

        TWCR = (1 << TWINT) | (1 << TWEN);
        while (!(TWCR & (1 << TWINT)))
            ;

        if ((TWSR & TW_STATUS_MASK) != TW_MR_DATA_NACK)
            ft_error(ERROR_6);

        master_state_receive();

        TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);

        _delay_ms(5);
    }
}