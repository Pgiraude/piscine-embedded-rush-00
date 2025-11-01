#ifndef TWI_H
#define TWI_H

#include <avr/io.h>
#include <util/delay.h>
#include <util/twi.h>

#define WRITE		0
#define READ		1
#define SLAVE_ADDR	0x20
#define MASTER_ADDR	0x00

#define SUCCESS		0
#define ERROR_1		1
#define ERROR_2		2

#define START		0x08
#define MT_SLA_ACK	0x18 // Master Transmit, Slave Address with bit write, Acknowledge
#define MT_DATA_ACK	0x28 // Master Transmit an octet to slave, and get ACK from slave

#define SLAVE_LISTEN_START	0x60
#define SLAVE_LISTEN_STOP	0xA0

void TWI_init(uint8_t my_address);
void ft_info(uint8_t status);

#endif