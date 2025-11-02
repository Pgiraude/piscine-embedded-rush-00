#include "TWI.h"

void TWI_init(uint8_t my_address) {
	DDRC &= ~((1 << PC4) | (1 << PC5)); //	SDA/SCL input
	PORTC |= (1 << PC4) | (1 << PC5);	//	enable pull-ups
	
	TWAR = (my_address << 1); // save the address in [7-1] bits, bit [0] is for General Call Enable

	TWBR = 72; // F_SCL = 100kHz
	TWSR = 0x00;  // sets prescaler bits TWPS1 and TWPS0 to zero, so TWPS = 0 (prescaler = 1)
	// F_SCL = F_CPU / (16 + 2 * TWBR * 4^TWPS)
}

void ft_error(uint8_t status) 
{
	DDRD |= (1 << PD5);
	PORTD |= (1 << PD5);

	DDRB |= (1 << PB0) | (1 << PB1) | (1 << PB2);

	PORTB |= (status & 0x07);

	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);

	while (1);
}

void got_hit() {
	DDRD |= (1 << PD3);
	PORTD |= (1 << PD3);
	_delay_ms(20);
	PORTD &= ~(1 << PD3);
}
