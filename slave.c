#include "TWI.h"

 void slave_listen(void) {
    while (1) {
        // 1. Attente d’un événement TWI (adresse, data, STOP…)
        while (!(TWCR & (1 << TWINT))) {
        }
        uint8_t status = TWSR & 0xF8;

        switch (status) {
            case SLAVE_LISTEN_START:
                ft_info(SUCCESS);
                break;
            case SLAVE_LISTEN_STOP:
                ft_info(ERROR_1);
                break;
            default:
                break;
        }
    }
}

int main() {
    TWI_init(SLAVE_ADDR);

    TWCR = (1 << TWEA) | (1 << TWEN);
    TWCR = (1 << TWINT); // to set up ?

    while (!(TWCR & (1 << TWINT))) {
    }

    ft_info(SUCCESS);
    if ((TWSR & 0xF8) != MT_DATA_ACK) {
        ft_info(ERROR_2);
        return;
    }

}
