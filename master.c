#include "TWI.h"
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

void main(void) {
	DDRB |= (1 << PB4); // show master
	PORTB |= (1 << PB4);

	TWI_init(MASTER_ADDR);

	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN); // send START condition
	while (!(TWCR & (1 << TWINT))); // wait for TWINT flag set, this indicates that the START condition has been transmitted

	if ((TWSR & 0xF8) != START) //check value of TWI Status Register. Mask prescaler bits
		ft_info(ERROR_1);
	
	TWI_send_address(SLAVE_ADDR, WRITE);

	if ((TWSR & 0xF8) != MT_SLA_ACK) // check TWI Status Register
		ft_info(ERROR_2);
	
	TWI_send_data('Z');
	
	while (!(TWCR & (1 << TWINT))); 

	if ((TWSR & 0xF8) != MT_DATA_ACK);

	TWCR = (1 << TWINT) | (1 << TWEN) | (1 <<TWSTO);

	ft_info(SUCCESS);
}