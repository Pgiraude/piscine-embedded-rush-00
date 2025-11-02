#include "TWI.h"

ISR(TIMER1_COMPA_vect) {
    game_state++;
    switch (game_state) {
    case 1:
        DDRB |= (1 << PB0);
        PORTB |= (1 << PB0);
        break;
    case 2:
        DDRB |= (1 << PB1);
        PORTB |= (1 << PB1);
        break;
    case 3:
        DDRB |= (1 << PB2);
        PORTB |= (1 << PB2);
        break;
    case 4:
        DDRB |= (1 << PB4);
        PORTB |= (1 << PB4);
        break;
    default:
        DDRB = 0x00;
        PORTB = 0x00;
        game_state = 0;
        TIMSK1 &= ~(1 << OCIE1A); // Disable timer interrupt
    }
}

void init_game_timer(void) {
    TCCR1B = (1 << CS12) | (1 << CS10); // Set prescaler to 1024
    TCCR1B |= (1 << WGM12);             // Set timer mode to CTC
    OCR1A = 15625; // fclk = 16000000 /(1024 * (1 + 15625)) = 1Hz
}

void set_game_start(void) {
    game_state = GAME_COUNTDOWN;
    TIMSK1 |= (1 << OCIE1A);
}
