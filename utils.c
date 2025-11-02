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
		PORTD |= (1 <<PD3)| (1 << PD5) | (1 << PD6);
	}
	else if (color == BLUE) {
		PORTD = (1 << PD3);
		PORTD &= ~((1 << PD5) | (1 << PD6));
	}
	else if (color == GREEN) {
		PORTD = (1 << PD6);
		PORTD &= ~((1 << PD3) | (1 << PD5));
	}
	else if (color == RED) {
		PORTD = (1 << PD5);
		PORTD &= ~((1 << PD3) | (1 << PD6));
	}
	else if (color == BLACK) {
		PORTD &= ~((1 << PD3) | (1 << PD5) | (1 << PD6));
	}
	else if (color == LED1) {
		PORTB = (1 << PB0);
		PORTB &= ~((1 << PB1) | (1 << PB2) | (1 << PB4));
	}
	else if (color == LED2) {
		PORTB = (1 << PB1);
		PORTB &= ~((1 << PB0) | (1 << PB2) | (1 << PB4));
	}
	else if (color == LED3) {
		PORTB = (1 << PB2);
		PORTB &= ~((1 << PB0) | (1 << PB1) | (1 << PB4));
	}
	else if (color == LED4) {
		PORTB = (1 << PB4);
		PORTB &= ~((1 << PB0) | (1 << PB1) | (1 << PB2));
	}
}

void end_game(uint8_t result) {
    if (GAME_OVER != game_state)
        return;
    if (result == WON) {
        DDRD |= (1 << PD6);
        PORTD |= (1 << PD6);
    } else if (result == LOST) {
        DDRD |= (1 << PD5);
        PORTD |= (1 << PD5);
    }
    _delay_ms(2000);
    PORTD = 0x00;
    DDRD = 0x00;
    button_pressed = 0; // Reset button state
    game_state = GAME_WAITING_PLAYER;
}
