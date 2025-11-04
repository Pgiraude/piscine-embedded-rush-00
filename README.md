# Rush 00: Multiplayer

## Overview

Multiplayer reaction game using I2C/TWI communication between two ATmega328P microcontrollers. Create a timing-based competitive game where speed and synchronization matter.

## Theoretical Concepts

**I2C/TWI Protocol**: Two-Wire Interface for inter-chip communication (SDA/SCL)  
**Master/Slave Architecture**: One MCU controls communication (master), the other responds (slave)  
**Clock Frequency**: SCL = 100kHz via TWBR register configuration  
**Status Codes**: TWSR register provides communication state (TW_START, TW_MT_SLA_ACK, etc.)  
**Game State Machine**: Synchronize game phases across two independent systems

---

## Exercise 00: Howdy Cowboy

**Goal**: Create a reaction-time multiplayer game with two microcontrollers communicating via I2C.

**Requirements**:

- Connect two devkits via I2C cable (SDA/SCL)
- Same code flashed on both MCUs
- Automatic master/slave role assignment
- Game synchronization via TWI protocol
- 100kHz I2C frequency
- Handle reconnection if one MCU restarts

**Game Rules**:

1. Both players press button to signal ready
2. Countdown displayed on LEDs (4 LEDs counting down)
3. After countdown, first player to press button wins
4. Pressing button during countdown = instant loss
5. Visual feedback (LED colors) indicates winner/loser
6. Game can restart automatically

**Key Concepts**:

- **Role Detection**: Use button or I2C status to determine master/slave dynamically
- **State Synchronization**: Master broadcasts game state, slave responds accordingly
- **Timing Critical**: Button press timing decides winner
- **Robust Communication**: Handle errors and reconnections gracefully

**I2C Implementation Notes**:

- Master: Initiates all communication (TWSTA), controls game flow
- Slave: Listens on address 0x12, responds to master requests
- Bidirectional: Master reads slave status, slave reads master commands
- Status bytes: Define protocol (READY, START, BUTTON_PRESSED, etc.)

---

## Skills Developed

- I2C/TWI protocol implementation
- Master/Slave communication patterns
- Distributed system synchronization
- State machine design across multiple devices
- Real-time timing and interrupts
- Error handling in networked systems

## Hardware Connections

```
MCU1 <--> MCU2
SDA  <--> SDA
SCL  <--> SCL
GND  <--> GND
```

## Formulas

```
I2C Frequency: F_SCL = F_CPU / (16 + 2 × TWBR × 4^TWPS)
For 100kHz: TWBR = 72, TWPS = 0 (prescaler = 1)
```
