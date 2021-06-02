/*
 Titel: C Labor 2 Aufgabe 2
 Laborgruppe: A9
 
 Lars Jelschen, Matrikelnummer: 5100174
 Marvin Klusmeyer, Matrikelnummer: 5093298
*/

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>

// LED Pin als Variable setzen
#define LED1 5 
int main(void){
	 // Interne LED an PB5
	DDRB |= 1<<LED1;
	
	while (1){
		// wartet auf 1s
		_delay_ms(1000); 
		// toggle PB5
		PORTB ^= (1<<LED1); 
	}
}