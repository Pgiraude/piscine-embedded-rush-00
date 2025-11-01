#include <avr/io.h>

#define SLAVE_ADDRESS 0x10 


void init_twi(void) {
    // TWCR (p235) -> TWEA pour ACK auto, TWEN active TWI
    TWCR = (1 << TWEA) | (1 << TWEN) | (1 << TWINT);
    // Need TWINT to be set 1 apparently
}


void init_slave(void) {
    // TWAR (p232) bits 7..1 slave
    // last bit= 0 because dont want to handle general call
    TWAR = (SLAVE_ADDRESS << 1);
    // TWCR (p235) -> TWEA pour ACK auto, TWEN active TWI
    TWCR = (1 << TWEA) | (1 << TWEN) | (1 << TWINT);
    // Need TWINT to be set 1 apparently
}

int main(void) {
    init_slave();

    while (1) {
    }

    return 0;
}