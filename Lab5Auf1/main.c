/*
 * Lab5Auf1.c
 *
 * Created: 2021-06-13 00:00:24
 * Author : Lars
 */ 

#define F_CPU	16000000

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/twi.h>
#include <util/delay.h>
#include "TWIPrimitives.c"
#include "DS3231.c"

int zeahler = 0;

ISR(INT0_vect){
	//zeahler bis 60
	if(zeahler == 60){
		zeahler = 0; //setzt den zeahler zurueck
	}else{
		zeahler += 1; //erhoht den zeahler um 1
	}

	PORTC &= ~(1<<0); //setzt PORTC zurueck
	PORTC |= (zeahler & 0x20)>>5; //setzt das 6.bit in PORTC
	PORTB = (zeahler & 0x1F); //setzt die Restlichen bits in PORTB
	ResetAlarm1(); //setzt das interrupt in der RTC zurueck
}

int main(void){
	DDRD = 0x00; // Port als Input
	DDRB = 0xFF; // Port als Ausgang
	DDRC = 1<<0; //Port als Ausgang

	TWIInit(); //TWI-Bus init.
    DS3231StartOsc(); // aktivieren der RTC clock
	
	//festlegen der zeitvariablen
	uint8_t s = 12;
	uint8_t m = 12;
	uint8_t h = 12;
	uint8_t date = 1; // SUNDAY
	uint8_t month = 6; //JUNE
	uint8_t year = 21;
	uint8_t dayOfWeek = 7;
	DS3231SetTimeAndDate(h, m, s, dayOfWeek, date, month, year); //setzen der Zeit in der RTC

	SetAlarm1Rate(0x0F); //alarmrate auf sekuendlich setzen
	
	EnableAlarm1(); // Alarm 1 Interrupt aktivieren
	EnableIntSqwt(); // Interrupts auf den Ausgang belegen

	PORTD = 1<<2; // Pullup at Interruptpin
	EICRA = 1<<1; // INT0 at Falling edge
	EIMSK = 1<<0; //enable interrupt mask
	sei(); // global Interrupts einschalten


    while (1) {}
}

