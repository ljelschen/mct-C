/*
 Titel: C Labor 2 Aufgabe 3
 Laborgruppe: A9
 
 Lars Jelschen, Matrikelnummer: 5100174
 Marvin Klusmeyer, Matrikelnummer: 5093298
*/


#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>

#define LED1 6

int main(void)
{
	// ausgang für OCR0A setzen
	DDRD |= (1<<LED1); 
	// fast PWM mode aktivieren
	TCCR0A |=  (1<<COM0A1) | (1<<WGM00) | (1<<WGM01); 
	// Timer0 einschalten
	TCCR0B |= (1<<CS00); 
	// start Wert für Dimmer festlegen
	int dimmer = 1; 

	while(1){
		// Wiederholt den Dimmvorgang
		while(dimmer < 100){ 
			//dimm Wert um 1 erhoehten
			dimmer += 1; 
			//OCR0A setzen
			OCR0A = (255/100) * dimmer; 
			// 10ms Pause
			_delay_ms(10); 
		}
		// vorgang wiederholen
		while(dimmer > 1){
			// von den dimm Wert 1 abzihen
			dimmer -= 1; 
			OCR0A = (255/100) * dimmer;
			_delay_ms(10);
		}

		dimmer = 0;
	}
}


