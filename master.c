#include "TWI.h"

void lights(uint8_t n)
{
	if (n == 1)
	{
		DDRD |= (1 << PD6);
		PORTD |= (1 << PD6);
	}
	else if (n == 0)
	{
		PORTD &= ~(1 << PD6);
		PORTD &= ~(1 << PD5);
	}
	else if (n == 2)
	{
		DDRD |= (1 << PD5) | (1 << PD6);
		PORTD |= (1 << PD5) | (1 << PD6);
	}
}

void checkAllValues(uint8_t val) {
    for (uint16_t i = 130; i <= 132; i++) {
        if (val == i) {
            got_hit();
            break;
        }
    }
}

void main() {
	TWI_init(0x00);

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
	
		TWDR = 'A';
		TWCR = (1 << TWINT) | (1 << TWEN);
	
		while (!(TWCR & (1 << TWINT)));
	
		if ((TWSR & TW_STATUS_MASK) != TW_MT_DATA_ACK)
			ft_error(ERROR_3);
	
		TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
	
		lights(1);
		_delay_ms(1000);
		lights(0);
		_delay_ms(1000);

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

		_delay_ms(100);
		lights(2);
		_delay_ms(1000);
		lights(0);
		_delay_ms(1000);
	}
}