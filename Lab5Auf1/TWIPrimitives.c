#include "TWIPrimitives.h"

void TWIInit(void) {
    TWSR = 0x00; // Prescaler Value 0x00
    TWBR = 0x48; // TWI clock 100 kHz 0x48
    TWCR = (1<<TWEN); // Enable TWI
    DDRB = 1<<5;
}

void TWIStart(void) {
    PORTB = 0<<5;
    TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
    while (!(TWCR & (1<<TWINT)));
}

void TWIStop(void) {
    TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN); 
    while(TWCR & (1<<TWSTO));
}

void TWIWrite(uint8_t data) { 
    TWDR = data;
    TWCR = (1<<TWINT)|(1<<TWEN);
    while (!(TWCR & (1<<TWINT)));
}

uint8_t TWIReadACK(void) {
    TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
    while (!(TWCR & (1<<TWINT)));
    return TWDR;
}

uint8_t TWIReadNACK(void) {
    TWCR = (1<<TWINT)|(1<<TWEN);
    while (!(TWCR & (1<<TWINT)));
    return TWDR;
}

uint8_t TWIGetStatus(void) {
    uint8_t status;
    status = TWSR & 0xF8;
    return status;
}

void TWIError(void) {
    // Add your error routine here
    // e.g. turn red LED on
    PORTB = 1<<5;
    TWIStop();	
}
