#include "TWI.h"
// MASTER 

uint8_t direction = TRANSMIT;
uint8_t button_pressed = 0;

void TWI_bus_recover(void) {
    // Temporarily disable TWI to manually control pins
    TWCR &= ~(1 << TWEN);

    DDRC |= (1 << PC5); // SCL as output
    DDRC &= ~(1 << PC4); // SDA as input
    PORTC |= (1 << PC4); // enable pull-up on SDA

    // Clock SCL up to 9 times until SDA goes high
    for (uint8_t i = 0; i < 9 && !(PINC & (1 << PC4)); i++) {
        PORTC &= ~(1 << PC5); // SCL low
        _delay_us(5);
        PORTC |= (1 << PC5);  // SCL high
        _delay_us(5);
    }

    // Generate a STOP condition manually
    DDRC |= (1 << PC4);  // SDA output
    PORTC &= ~(1 << PC4); // SDA low
    _delay_us(5);
    PORTC |= (1 << PC5);  // SCL high
    _delay_us(5);
    PORTC |= (1 << PC4);  // SDA high while SCL high
    _delay_us(5);

    // Release lines
    DDRC &= ~((1 << PC4) | (1 << PC5)); // inputs again
    PORTC |= (1 << PC4) | (1 << PC5);   // pull-ups

    // Re-enable TWI
    TWCR |= (1 << TWEN);
}

void TWI_send_start() {
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);

	while (!(TWCR & (1 << TWINT))); //START condition has been transmitted

	if ((TWSR & TW_STATUS_MASK) != TW_START) //check Status Register
		ft_error(ERROR_1);
}

void TWI_send_address(uint8_t addr, uint8_t rw)
{
	TWDR = (addr << 1) | rw; // combine SLA and R/W bit  (0 for write, 1 for read)
	TWCR = (1 << TWINT) | (1 << TWEN); // clear TWINT flag (start next operation)
	while (!(TWCR & (1 << TWINT))); // wait for completion
	if (rw == WRITE) {
		if ((TWSR & TW_STATUS_MASK) != TW_MT_SLA_ACK)
			ft_error(ERROR_2);
	}
	if (rw == READ) {
		if ((TWSR & TW_STATUS_MASK) != TW_MR_SLA_ACK)
			ft_error(ERROR_3);
		TWCR |= (1 <<TWEA);
	}	
}

void TWI_send_data(char c)
{
	TWDR = c;
	TWCR = (1 << TWINT) | (1 << TWEN);
}
/*
void init_timer1(void)
{
	TCCR1B |= (1 << WGM12) | (1 << CS12) | (1 << CS10); // CTC mode, 1024 prescalar
	TIMSK1 |= (1 << OCIE1A); // enable interrupt

	OCR1A = 30000;
}

ISR(TIMER1_COMPA_vect) {
	if (direction == TRANSMIT)
		direction = RECIVE;
	else 
		direction = TRANSMIT;
}*/

ISR(PCINT2_vect)
{
    static int pressed = 0;
    pressed = (pressed + 1) % 2; //skips one pin change interrupt (when releasing)

    if (pressed == 1)
    {
        button_pressed = 1;
    }

    _delay_ms(20);
    PCIFR |= (1 << PCIF2); // clear any pending interrupts
}

void master_transmit() 
{
	PORTB ^= (1 << PB4);
	TWI_send_start();
	TWI_send_address(SLAVE_ADDR, WRITE);
	
	while (1)
	{
		if (button_pressed == 1) {
			TWDR = MASTER_BUTTON_PRESSED;
			while (!(TWCR & (1 << TWINT)));
			if ((TWSR & TW_STATUS_MASK) != TW_MT_DATA_ACK)
				ft_error(ERROR_4);
			TWCR = (1 << TWINT) | (1 << TWEN);
			button_pressed = 0;
		}
		if (direction != TRANSMIT)
			return;
	}
}

void master_recive() 
{
	PORTB ^= (1 << PB4);
	TWI_send_start();
	TWI_send_address(SLAVE_ADDR, READ);

	while (1)
	{
		if (TWCR & (1 << TWINT)) {
			if ((TWSR & TW_STATUS_MASK) != TW_MR_DATA_ACK)
				ft_error(ERROR_5);
			if (TWDR == SLAVE_BUTTON_PRESSED)
				got_hit();
			TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
		}
		if (direction != RECIVE)
			return;
	}
}

void main(void)
{
	LED_MASTER_ON;
	
	TWI_init(MASTER_ADDR);

	TWI_send_start();
}