/*
 Titel: C Labor 2 Aufgabe 1
 Laborgruppe: A9
 
 Lars Jelschen, Matrikelnummer: 5100174
 Marvin Klusmeyer, Matrikelnummer: 5093298
*/

#include <avr/io.h>


int main(void){
	// dimmwert einstellen
	int dimmer = 50; 
	//OCR0A als Ausgang setzen
	DDRD |= (1<<6); 
	// fast PWM mode aktivieren
	TCCR0A |=  (1<<COM0A1) | (1<<WGM00) | (1<<WGM01); 
	//Timer0 einschalten
	TCCR0B |= (1<<CS00);
	//OCR0A Wert setzen
	OCR0A = (255/100) * dimmer; 
	while(1){}
}