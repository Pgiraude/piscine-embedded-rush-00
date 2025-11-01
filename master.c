#include "TWI.h"

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

	while (1) {
		lights(1);
		TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
	
		while (!(TWCR & (1 << TWINT)));
	
		if ((TWSR & TW_STATUS_MASK) != TW_START)
			ft_error(ERROR_1);
	
		TWDR = (SLAVE_ADDR << 1) | WRITE;
		TWCR = (1 << TWINT) | (1 << TWEN);
	
		while (!(TWCR & (1 << TWINT)));
	
		if ((TWSR & TW_STATUS_MASK) != TW_MT_SLA_ACK)
			ft_error(ERROR_2);
	
		TWDR = 'A';
		TWCR = (1 << TWINT) | (1 << TWEN);
	
		while (!(TWCR & (1 << TWINT)));
	
		if ((TWSR & TW_STATUS_MASK) != TW_MT_DATA_ACK)
			ft_error(ERROR_3);
	
		TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
	
		_delay_ms(1000);
		lights(2);

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
		if (data == 'B')
			got_hit();

		TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);

		_delay_ms(1000);
	}
}