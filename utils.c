#include "TWI.h"

void TWI_init(uint8_t my_address) {
	DDRC &= ~((1 << PC4) | (1 << PC5)); //	SDA/SCL input
	PORTC |= (1 << PC4) | (1 << PC5);	//	enable pull-ups
	
	TWAR = (my_address << 1); // save the address in [7-1] bits, bit [0] is for General Call Enable

	TWBR = 72; // F_SCL = 100kHz
	TWSR = 0x00;  // sets prescaler bits TWPS1 and TWPS0 to zero, so TWPS = 0 (prescaler = 1)
	// F_SCL = F_CPU / (16 + 2 * TWBR * 4^TWPS)

	TWCR = (1 << TWEN); // enable TWI
}

void ft_info(uint8_t status) {

	switch (status) {
		case ERROR_1:
			DDRD |= (1 << PD5);
			PORTD |= (1 << PD5);
			break;
		case ERROR_2:
			DDRD |= (1 << PD5) | (1 << PD6);
			PORTD |= (1 << PD5) | (1 << PD6);
			break;
		case SUCCESS:
			DDRD |= (1 << PD6);
			PORTD |= (1 << PD6);
			break;
	}
}
