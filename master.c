#include "TWI.h"

uint8_t button_pressed = 0;
uint8_t someone_won = 0;

ISR(PCINT2_vect)
{
    static int pressed = 0;
    pressed = (pressed + 1) % 2; //skips one pin change interrupt (when releasing)

    if (pressed == 1 && button_pressed == 0)
	{
		button_pressed = 1;
	}

    _delay_ms(20);
    PCIFR |= (1 << PCIF2); // clear any pending interrupts
}

void lights(uint8_t n)
{
	DDRB |= (1 << PB1) | (1 << PB2);
	if (n == 1)
	{
		PORTB |= (1 << PB1);
		PORTB &= ~(1 << PB2);
	}
	else if (n == 2)
	{
		PORTB |= (1 << PB2);
		PORTB &= ~(1 << PB1);
	}
}

void main() {
	TWI_init(0x00);

	sei(); // Enable global interrupts
	PCICR |= (1 << PCIE2); // Enable pin change interrupt for PCINT[23:16]
	PCMSK2 |= (1 << PCINT20); // Enable pin change interrupt for PCINT20 (PD2)

	while (1) {
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
			button_pressed = 0;
			TWDR = MASTER_BUTTON_PRESSED;
			someone_won = 1;
		}
		else
			TWDR = 'A';

		TWCR = (1 << TWINT) | (1 << TWEN);
	
		while (!(TWCR & (1 << TWINT)));
	
		if ((TWSR & TW_STATUS_MASK) != TW_MT_DATA_ACK)
			ft_error(ERROR_3);
		
		TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);

		if (someone_won == 1)
			player_won(MASTER);
	
		_delay_ms(5);

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
		if (data == SLAVE_BUTTON_PRESSED)
			someone_won = 1;

		TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);

		if (someone_won == 1)
			player_won(SLAVE);
		
		_delay_ms(5);
	}
}