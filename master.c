#include "TWI.h"
#include <avr/interrupt.h>
// MASTER 

void TWI_send_address(uint8_t addr, uint8_t rw)
{
	TWDR = (addr << 1) | rw; // combine SLA and R/W bit  (0 for write, 1 for read)
	TWCR = (1 << TWINT) | (1 << TWEN); // clear TWINT flag (start next operation)
	while (!(TWCR & (1 << TWINT))); // wait for completion
}

void TWI_send_data(char c)
{
	TWDR = c;
	TWCR = (1 << TWINT) | (1 << TWEN);
}

char TWI_receive_data(void)
{
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN); // send START condition
	while (!(TWCR & (1 << TWINT))); // wait for TWINT flag set, this indicates that the START condition has been transmitted

	if ((TWSR & 0xF8) != START){
		ft_info(ERROR_2);
		return 0;
	} //check value of TWI Status Register. Mask prescaler bits

	TWI_send_address(SLAVE_ADDR, READ);

	if ((TWSR & 0xF8) != TW_MR_SLA_ACK) {
		ft_info(ERROR_2);
	} // check TWI Status Register
	
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
	if (TWCR & (1 << TWINT)) {
		return TWDR;

	}

    return 0;
}

void TWI_transmit_data(void) {
	 // send START condition
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN); // send START condition
	while (!(TWCR & (1 << TWINT))) {} // wait for TWINT flag set, this indicates that the START condition has been transmitted
	
	if ((TWSR & 0xF8) != START) {
		ft_info(ERROR_1);
	} //check value of TWI Status Register. Mask prescaler bits
	
	TWI_send_address(SLAVE_ADDR, WRITE);
	if ((TWSR & 0xF8) != MT_SLA_ACK) // check TWI Status Register
		ft_info(ERROR_3);
	
	TWI_send_data('Z');
	while (!(TWCR & (1 << TWINT))) {}

	if ((TWSR & 0xF8) != MT_DATA_ACK) {
		ft_info(ERROR_3);
	}

	TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN); // send STOP condition
}

void main(void) {

	DDRB |= (1 << PB4); // show master
	PORTB |= (1 << PB4);
	sei();

	TWI_init(MASTER_ADDR);
	char data = 0;


	while (1) {
		TWI_transmit_data();
		// data = 0;
		// // data = TWI_receive_data();
		// if (data)
		// 	ft_info(SUCCESS);
		ft_info(SUCCESS);
	}
}