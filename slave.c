#include "TWI.h"
/*
uint8_t button_pressed = 0;

uint8_t TWI_check_bus(void)
{
    while (!(TWCR & (1 << TWINT)));

    uint8_t status = TWSR & TW_STATUS_MASK;

    if (status == TW_SR_SLA_ACK)
        return SLAVE_RECIVE;
    if (status == TW_ST_SLA_ACK)
        return SLAVE_TRANSMIT;
    if (status == TW_SR_DATA_ACK)
        return TWDR;
}

int main(void)
{
    //LED_SLAVE_ON;
    got_hit();

    TWI_init(SLAVE_ADDR);
    TWCR_SLAVE_RESET;

    uint8_t data = 0;
    uint8_t tw_action = SLAVE_RECIVE;

    while (1)
    {
        data = TWI_check_bus();
        if (data == SLAVE_RECIVE)
            tw_action = SLAVE_RECIVE;
        else if (data == SLAVE_TRANSMIT)
            tw_action = SLAVE_TRANSMIT;

        if (tw_action == SLAVE_RECIVE) {
            if (data == MASTER_BUTTON_PRESSED)
                got_hit();
        }
        if (tw_action == SLAVE_TRANSMIT && button_pressed) {
            TWDR = SLAVE_BUTTON_PRESSED;
            TWCR_SLAVE_RESET;
        }
    }
}
*/

void TWI_init_slave(uint8_t address) {
    // Set own address
    TWAR = (address << 1);

    // Enable TWI, ACK, and clear interrupt flag
    TWCR = (1 << TWEN) | (1 << TWEA) | (1 << TWINT);

    // Optional: enable weak pull-ups
    PORTC |= (1 << PC4) | (1 << PC5);
}

int main(void) {
    TWI_init_slave(SLAVE_ADDR);

    // Main loop — just stay ready, do nothing
    while (1) {
        // Always re-arm ACK and clear TWINT if set
        if (TWCR & (1 << TWINT)) {
            TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN);
        }
    }
}



