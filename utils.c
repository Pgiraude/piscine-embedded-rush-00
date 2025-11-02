#include "TWI.h"

void interrupt_init() {
    sei();
    PCICR |= (1 << PCIE2);    // enable Pin Change Interrupt for group D
    PCMSK2 |= (1 << PCINT18); // enable specifically for PD2
}

ISR(PCINT2_vect) {
    static int pressed = 0;
    pressed = (pressed + 1) % 2;

    if (pressed == 1 && button_pressed == 0)
        button_pressed = 1;

    _delay_ms(20);
    PCIFR |= (1 << PCIF2);
}

void ft_error(uint8_t status) {
    DDRD |= (1 << PD5);
    PORTD |= (1 << PD5);

    DDRB |= (1 << PB0) | (1 << PB1) | (1 << PB2);
    PORTB |= (status & 0x07);

    TWCR = 0;
    while (1)
        ;
}
void light(uint8_t color) {
    DDRD |= (1 << PD3) | (1 << PD5) | (1 << PD6);
    DDRB |= (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB4);
    if (color == WHITE) {
        PORTD |= (1 << PD3) | (1 << PD5) | (1 << PD6);
    } else if (color == BLUE) {
        PORTD = (1 << PD3);
        PORTD &= ~((1 << PD5) | (1 << PD6));
    } else if (color == GREEN) {
        PORTD = (1 << PD6);
        PORTD &= ~((1 << PD3) | (1 << PD5));
    } else if (color == RED) {
        PORTD = (1 << PD5);
        PORTD &= ~((1 << PD3) | (1 << PD6));
    } else if (color == BLACK) {
        PORTD &= ~((1 << PD3) | (1 << PD5) | (1 << PD6));
    } else if (color == LED1) {
        PORTB = (1 << PB0);
        PORTB &= ~((1 << PB1) | (1 << PB2) | (1 << PB4));
    } else if (color == LED2) {
        PORTB = (1 << PB1);
        PORTB &= ~((1 << PB0) | (1 << PB2) | (1 << PB4));
    } else if (color == LED3) {
        PORTB = (1 << PB2);
        PORTB &= ~((1 << PB0) | (1 << PB1) | (1 << PB4));
    } else if (color == LED4) {
        PORTB = (1 << PB4);
        PORTB &= ~((1 << PB0) | (1 << PB1) | (1 << PB2));
    }
}

void end_game(uint8_t result) {
    if (result == UNDECIDED) {
        return;
    }
    if (result == WON) {
        light(GREEN);
    } else if (result == LOST) {
        light(RED);
    }
    _delay_ms(3000);
    game_state = GAME_WAITING_PLAYER;
    your_are = UNDECIDED;
    light(BLACK);
}

void call_timer() {
    // Set CTC mode (Clear Timer on Compare Match)
    TCCR1B |= (1 << WGM12);

    // Set compare value for 1 Hz (1 second)
    OCR1A = 15624;

    // Enable interrupt on compare match A
    TIMSK1 |= (1 << OCIE1A);

    // Set prescaler to 1024 and start the timer
    TCCR1B |= (1 << CS12) | (1 << CS10);

    // Enable global interrupts
    sei();

    DDRB |= (1 << PB1) | (1 << PB2) | (1 << PB0) | (1 << PB4);
    PORTB |= (1 << PB1) | (1 << PB2) | (1 << PB0) | (1 << PB4);
}

uint8_t count_down = 0;

void end_timer() {
    DDRB = 0x00;
    PORTB = 0x00;
    count_down = 0;
    TIMSK1 = 0; // Disable timer interrupt
    TCCR1B = 0;
}

ISR(TIMER1_COMPA_vect) {
    count_down++;
    switch (count_down) {
    case 1:
        PORTB &= ~(1 << PB4);
        break;
    case 2:
        PORTB &= ~(1 << PB2);
        break;
    case 3:
        PORTB &= ~(1 << PB1);
        break;
    case 4:
        PORTB &= ~(1 << PB0);
        break;
    default:
        end_timer();
        game_state = GAME_STARTED;
    }
}
