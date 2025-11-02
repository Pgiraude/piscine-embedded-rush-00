#include "TWI.h"

int main(void) {
    uint8_t f_game_over = 0;

    TWI_init(SLAVE_ADDR);
    interrupt_init();
    
    TWCR = (1<<TWEN) | (1<<TWEA) | (1<<TWINT);

    ready_flash();
    while(1) {
        if(TWCR & (1<<TWINT)) {
            if (f_game_over == 1)
                end_game(WON);
            uint8_t status = TWSR & TW_STATUS_MASK;

            switch(status) 
            {
                case TW_ST_SLA_ACK:
                case TW_ST_DATA_ACK:
                    if (button_pressed == 1) {
                        TWDR = OPPONENT_BUTTON_PRESSED;
                        f_game_over = 1;
                    } else TWDR = 'A';
                    break;
        
                case TW_ST_DATA_NACK:
                case TW_SR_STOP:
                case TW_SR_SLA_ACK:
                    break;

                case TW_SR_DATA_ACK:
                    if (TWDR == OPPONENT_BUTTON_PRESSED)
                        end_game(LOST);
                    break;
                default:
                    ft_error(ERROR_1);
            }
            TWCR = (1<<TWEN) | (1<<TWEA) | (1<<TWINT);
        }
    }
}
