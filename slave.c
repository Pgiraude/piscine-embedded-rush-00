#include "TWI.h"
#define F_CPU 16000000UL

void TWI_init_slave(uint8_t address) {
    TWAR = (address << 1);          // Set own 7-bit address
    TWCR = (1<<TWEN) | (1<<TWEA) | (1<<TWINT); // Enable TWI, ACK, clear INT
}

void TWI_ready(void) {
    TWCR = (1<<TWEN) | (1<<TWEA) | (1<<TWINT);
}

int main(void) {
    TWI_init_slave(SLAVE_ADDR);

    uint8_t dummy = 'B'; // byte to send
    uint8_t received = 0;

    while(1) {
        if(TWCR & (1<<TWINT)) {
            uint8_t status = TWSR & 0xF8;

            switch(status) {
                // --- Master read request ---
                case 0xA8: // SLA+R received, ACK returned
                    TWDR = dummy;           // load the byte to send
                    TWCR = (1<<TWEN)|(1<<TWEA)|(1<<TWINT); // clear TWINT to start transmission
                    break;

                case 0xB8: // Data transmitted, ACK received
                    TWDR = dummy;           // next byte if needed
                    TWCR = (1<<TWEN)|(1<<TWEA)|(1<<TWINT);
                    break;

                case 0xC0: // Data transmitted, NACK received
                case 0xC8: // Last data transmitted, ACK received
                case 0xA0: // STOP or repeated START
                    TWI_ready();            // prepare for next transaction
                    break;

                // --- Master write request ---
                case 0x60: // SLA+W received
                case 0x80: // Data received
                case 0x88: // Data received, NACK
                    received = TWDR;           // read byte to clear buffer
                    if (received == 'A') {
                        got_hit();
                    }
                    TWI_ready();
                    break;

                default:                     // unexpected states
                    TWI_ready();
                    break;
            }
        }
    }
}
