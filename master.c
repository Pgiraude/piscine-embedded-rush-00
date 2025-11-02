#include "TWI.h"

void master_init() {
	TWBR = 72;   // F_SCL = 100kHz
    TWSR = 0x00; // sets prescaler bits TWPS1 and TWPS0 to zero, so TWPS = 0
                 // (prescaler = 1)
                 // F_SCL = F_CPU / (16 + 2 * TWBR * 4^TWPS)
	TWCR = (1 << TWEN) | (1 << TWINT);
}

void master_loop() {
    uint8_t screen_score = 1;
	uint8_t screen_countdown = 0;
	screen_playing = 0;
	uint8_t need_to_timer = 0;
	uint8_t slave_ready = 1;
	uint8_t master_ready = 1;
	uint8_t f_game_over = 0;
	
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

		if (screen_score == 1 && slave_ready == 1 && master_ready == 1) {
			TWDR = TIMER_NOW;
			screen_countdown = 1;
			light(BLACK);
			master_ready = 0;
			slave_ready = 0;
			screen_score = 0;
			need_to_timer = 1;
		}
        else if (button_pressed == 1 && screen_playing == 1) {
            TWDR = OPPONENT_BUTTON_PRESSED;
			light(GREEN);
            screen_playing = 0;
			screen_score = 1;
			button_pressed = 0;
			master_ready = 0;
			slave_ready = 0;
		} 
        else if (button_pressed == 1 && screen_countdown == 1) {
			TWDR = OPPONENT_LOST;
			stop_timer();
			light(RED);
			screen_playing = 0;
			screen_countdown = 0;
			screen_score = 1;
			master_ready = 0;
			slave_ready = 0;
			button_pressed = 0;
        } else
            TWDR = 'A';

		if (button_pressed == 1 && master_ready == 0 && screen_score == 1) {
			master_ready = 1;
			light(LED1);
			button_pressed = 0;
		}

        TWCR = (1 << TWINT) | (1 << TWEN);

        while (!(TWCR & (1 << TWINT)))
            ;

        if ((TWSR & TW_STATUS_MASK) != TW_MT_DATA_ACK)
            ft_error(ERROR_3);

        TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);

		if (need_to_timer == 1) {
			light(BLACK);
			call_timer();
			need_to_timer = 0;
		}

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
        if (data == OPPONENT_BUTTON_PRESSED && screen_playing == 1) {
			light(RED);
			screen_score = 1;
			master_ready = 0;
			slave_ready = 0;
			screen_playing = 0;
		}
		else if (data == OPPONENT_BUTTON_PRESSED && screen_score == 1)
		{
			slave_ready = 1;
		}
		else if (data == OPPONENT_LOST) {
			stop_timer();
			light(GREEN);
			master_ready = 0;
			slave_ready = 0;
			screen_countdown = 0;
			screen_score = 1;
		}

        TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);

        _delay_ms(5);
    }
}