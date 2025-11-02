#include "TWI.h"

void master_init() {
	TWBR = 72;   // F_SCL = 100kHz
    TWSR = 0x00; // sets prescaler bits TWPS1 and TWPS0 to zero, so TWPS = 0
                 // (prescaler = 1)
                 // F_SCL = F_CPU / (16 + 2 * TWBR * 4^TWPS)
	TWCR = (1 << TWEN) | (1 << TWINT);
}

void master_loop() {
    uint8_t f_game_over = 0;
	playing = 0;
	uint8_t need_to_timer = 1;
	
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

		if (need_to_timer == 1) {
			TWDR = TIMER_NOW;
		}
        else if (button_pressed == 1 && playing == 1) {
            TWDR = OPPONENT_BUTTON_PRESSED;
            f_game_over = 1;
        } else if (button_pressed == 1 && playing == 0) {
			TWDR = OPPONENT_LOST;
			f_game_over = 2;
        } else
            TWDR = 'A';

        TWCR = (1 << TWINT) | (1 << TWEN);

        while (!(TWCR & (1 << TWINT)))
            ;

        if ((TWSR & TW_STATUS_MASK) != TW_MT_DATA_ACK)
            ft_error(ERROR_3);

        TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);

		if (need_to_timer == 1) {
			call_timer();
			need_to_timer = 0;
		}
        if (f_game_over == 1)
            end_game(WON);
		if (f_game_over == 2)
			end_game(LOST);

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

        uint8_t data = TWDR;
        if (data == OPPONENT_BUTTON_PRESSED)
            f_game_over = 2;
		else if (data == OPPONENT_LOST)
			f_game_over = 1;

        TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);

        if (f_game_over == 2)
            end_game(LOST);
		if (f_game_over == 1)
			end_game(WON);

        _delay_ms(5);
    }
}