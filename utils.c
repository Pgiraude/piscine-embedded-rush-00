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
	if (status == SUCCESS) {
		DDRD |= (1 << PD6);
		PORTD |= (1 << PD6);
		_delay_ms(5);
		PORTD &= ~(1 << PD6);
		return;
	}

	if (status == DEBUG) {
		DDRD |= (1 << PD6) | (1 << PD5) | (1 << PD3);
		PORTD |= (1 << PD6) | (1 << PD5) | (1 << PD3);
		_delay_ms(1000);
		PORTD &= ~((1 << PD6) | (1 << PD5) | (1 << PD3));
		return;
	}

	DDRD |= (1 << PD5);
	PORTD |= (1 << PD5); 
	
	if (status == ERROR_1) {
		DDRB |= (1 << PB0);
		PORTB |= (1 << PB0);
	}
	else if (status == ERROR_2) {

		DDRB |= (1 << PB1);
		PORTB |= (1 << PB1);
	}
	else if (status == ERROR_3)
	{
		DDRB |= (1 << PB1) | (1 << PB0);
		PORTB |= (1 << PB1) | (1 << PB0);
	}

	while (1);
}
