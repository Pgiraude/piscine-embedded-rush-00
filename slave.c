#include "TWI.h"
/*
int main() {
    DDRB |= (1 << PB2); // show slave
    PORTB |= (1 << PB2);

    TWI_init(SLAVE_ADDR);
    
    TWCR = (1 << TWEA) | (1 << TWEN) | (1 << TWINT);
    
    while (!(TWCR & (1 << TWINT)));
    
    if ((TWSR & 0xF8) != MT_DATA_ACK)
        ft_info(ERROR_2);
    
    ft_info(SUCCESS);
}*/

void TWI_init_slave(uint8_t address)
{
    TWAR = (address << 1);                   // Load slave address into TWI Address Register
    TWCR = (1 << TWEN) | (1 << TWEA) | (1 << TWINT); // Enable TWI, ACK, and clear interrupt flag
}

uint8_t TWI_wait_for_data(void)
{
    // Wait for data or address match
    while (!(TWCR & (1 << TWINT)));          // Wait until TWINT is set

    uint8_t status = TWSR & 0xF8;            // Get status bits (mask prescaler)

    if (status == 0x60) { // Own SLA+W received, ACK returned
        // Wait for data
        TWCR = (1 << TWEN) | (1 << TWEA) | (1 << TWINT); // Ready to receive data
        while (!(TWCR & (1 << TWINT)));      // Wait until data received
        status = TWSR & 0xF8;
        if (status == 0x80) {
            uint8_t data = TWDR;             // Read received byte
            return data;                      // Return the received data
        }
    }

    return 0xFF; // No valid data received
}

int main(void)
{
    DDRB |= (1 << PB2); // show slave
    PORTB |= (1 << PB2);
    uint8_t received = 0;

    TWI_init_slave(SLAVE_ADDR);

    while (1)
    {
        received = TWI_wait_for_data();      // Wait for data from master

        // Example: toggle a pin when a byte is received
        if (received == 'Z') {
            ft_info(SUCCESS);
        }
        else
            ft_info(ERROR_1);
    }
}

