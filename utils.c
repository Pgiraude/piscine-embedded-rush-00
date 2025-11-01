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
	DDRB |= (1 << PB0) | (1 << PB1) | (1 << PB2);
	
}
