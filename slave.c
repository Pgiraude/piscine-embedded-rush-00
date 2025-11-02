#include "TWI.h"

uint8_t someone_won = 0;

int main(void) {
    TWI_init(SLAVE_ADDR);

    TWCR = (1<<TWEN) | (1<<TWEA) | (1<<TWINT);

    sei(); // Enable global interrupts

    PCICR |= (1 << PCIE2); // Enable pin change interrupt for PCINT[23:16]
    PCMSK2 |= (1 << PCINT20); // Enable pin change interrupt for PCINT20 (PD2)

    got_hit();
    while(1) {
        if(TWCR & (1<<TWINT)) {
            if (someone_won == 1)
                player_won(MASTER); // to be changed
            uint8_t status = TWSR & TW_STATUS_MASK;

            switch(status) 
            {
                case TW_ST_SLA_ACK:
                case TW_ST_DATA_ACK:
                    if (button_pressed == 1) {
                        button_pressed = 0;
                        TWDR = SLAVE_BUTTON_PRESSED;
                        someone_won = 1;
                    } else TWDR = 'A';
                    break;
        
                case TW_ST_DATA_NACK:
                case TW_SR_STOP:
                case TW_SR_SLA_ACK:
                    break;

                case TW_SR_DATA_ACK:
                    if (TWDR == MASTER_BUTTON_PRESSED)
                        player_won(SLAVE);
                    break;
                default:
                    ft_error(ERROR_1);
            }
            TWCR = (1<<TWEN) | (1<<TWEA) | (1<<TWINT);
        }
    }
}
