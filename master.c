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
	TWI_init(0x00);

	TWCR = (1 << TWINT) | (1 << TWSTA); // send START condition

	while (!(TWCR & (1 << TWINT))); // wait for TWINT flag set, this indicates that the START condition has been transmitted

	if ((TWSR & 0x11111000) != START) //check value of TWI Status Register. Mask prescaler bits
		ft_info(1);
	
	TWI_send_address(SLAVE_ADDRESS, WRITE);

	if ((TWSR & 0x11111000) != MT_SLA_ACK) // check TWI Status Register
		ft_info(2);
	
	TWI_send_data('Z');
	
	while (!(TWCR & (1 << TWIN))); 

	if ((TWSR & 11111000) != MT_DATA_ACK);

	TWCR = (1 << TWINT) | (1 << TWEN) | (1 <<TWSTO);

	ft_info(0);
}