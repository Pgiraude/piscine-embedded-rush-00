#include "TWI.h"
#define F_CPU 16000000UL

uint8_t button_pressed = 0;
uint8_t someone_won = 0;

ISR(PCINT2_vect)
{
    static int pressed = 0;
    pressed = (pressed + 1) % 2; //skips one pin change interrupt (when releasing)

    if (pressed == 1 && button_pressed == 0)
	{
		button_pressed = 1;
	}

    _delay_ms(20);
    PCIFR |= (1 << PCIF2); // clear any pending interrupts
}

void TWI_init_slave(uint8_t address) {
    TWAR = (address << 1);          // Set own 7-bit address
    TWCR = (1<<TWEN) | (1<<TWEA) | (1<<TWINT); // Enable TWI, ACK, clear INT
}

void TWI_ready(void) {
    TWCR = (1<<TWEN) | (1<<TWEA) | (1<<TWINT);
}

int main(void) {
    TWI_init_slave(SLAVE_ADDR);

    sei(); // Enable global interrupts
    PCICR |= (1 << PCIE2); // Enable pin change interrupt for PCINT[23:16]
    PCMSK2 |= (1 << PCINT20); // Enable pin change interrupt for PCINT20 (PD2)

    uint8_t dummy = 'B'; // byte to send
    uint8_t received = 0;

    while(1) {
        if(TWCR & (1<<TWINT)) {
            if (someone_won == 1)
                player_won(MASTER); // to be changed
            uint8_t status = TWSR & 0xF8;

            switch(status) {
                // --- Master read request ---
                case 0xA8: // SLA+R received, ACK returned
                    if (button_pressed == 1) {
                        button_pressed = 0;
                        TWDR = SLAVE_BUTTON_PRESSED;
                        someone_won = 1;
                    }
                    else
                    TWDR = dummy;         // load the byte to send
                    TWCR = (1<<TWEN)|(1<<TWEA)|(1<<TWINT); // clear TWINT to start transmission
                    break;

                case 0xB8: // Data transmitted, ACK received
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
                    if (received == MASTER_BUTTON_PRESSED) {
                        player_won(SLAVE);
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
