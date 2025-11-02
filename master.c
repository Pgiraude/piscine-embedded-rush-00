#include "TWI.h"

void main() {
	uint8_t f_game_over = 0;

	TWI_init(0x00);
	interrupt_init();

	ready_flash();
	while (1) {
		//	TRANSMITING
		TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
	
		while (!(TWCR & (1 << TWINT)));
	
		if ((TWSR & TW_STATUS_MASK) != TW_START)
			ft_error(ERROR_1);
	
		TWDR = (SLAVE_ADDR << 1) | WRITE;
		TWCR = (1 << TWINT) | (1 << TWEN);
	
		while (!(TWCR & (1 << TWINT)));
	
		if ((TWSR & TW_STATUS_MASK) != TW_MT_SLA_ACK)
			ft_error(ERROR_2);
	
		if (button_pressed == 1) {
			TWDR = OPPONENT_BUTTON_PRESSED;
			f_game_over = 1;
		}
		else TWDR = 'A';

		TWCR = (1 << TWINT) | (1 << TWEN);
	
		while (!(TWCR & (1 << TWINT)));
	
		if ((TWSR & TW_STATUS_MASK) != TW_MT_DATA_ACK)
			ft_error(ERROR_3);
		
		TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);

		if (f_game_over == 1)
			end_game(WON);
	
		_delay_ms(5);

		//	RECIVING
		TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);

		while (!(TWCR & (1 << TWINT)));
	
		if ((TWSR & TW_STATUS_MASK) != TW_START)
			ft_error(ERROR_4);

		TWDR = (SLAVE_ADDR << 1) | READ;
		TWCR = (1 << TWINT) | (1 << TWEN);

		while (!(TWCR & (1 << TWINT)));
	
		if ((TWSR & TW_STATUS_MASK) != TW_MR_SLA_ACK)
			ft_error(ERROR_5);

		TWCR = (1 << TWINT) | (1 << TWEN);
		while (!(TWCR & (1 << TWINT)));

		if ((TWSR & TW_STATUS_MASK) != TW_MR_DATA_NACK)
			ft_error(ERROR_6);

		uint8_t data = TWDR;
		if (data == OPPONENT_BUTTON_PRESSED)
			f_game_over = 1;

		TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);

		if (f_game_over == 1)
			end_game(LOST);
		
		_delay_ms(5);
	}
}