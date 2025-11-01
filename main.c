#include <avr/io.h>
#include <util/delay.h>

// ne marche pas ici
void detect_stop_condition(void) {
    while (TWCR & (1 << TWSTO)) {
    }

    DDRD |= (1 << PD3);
    PORTD |= (1 << PD3);
    _delay_ms(2000);
    PORTD &= ~(1 << PD3);
}

void waiting_start_condition(void) {
    // Wait for TWINT Flag set. This
    // indicates that the START
    // condition has been transmitted
    while (!(TWCR & (1<<TWINT)))
        ;
    DDRD |= (1 << PD5);
    PORTD |= (1 << PD5);
    _delay_ms(2000);
    PORTD &= ~(1 << PD5);
    // Check value of TWI Status Register. If status
}

int main(void) {
    // init start
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);

    DDRD |= (1 << PD6);
    PORTD |= (1 << PD6);
    _delay_ms(2000);
    PORTD &= ~(1 << PD6);
    _delay_ms(2000);


    waiting_start_condition();


    // stop request
    TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);

    _delay_ms(1000);

    detect_stop_condition();

    return 0;
}
