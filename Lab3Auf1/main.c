/*
 Titel: C Labor 3 Aufgabe 1
 Laborgruppe: A9
 
 Lars Jelschen, Matrikelnummer: 5100174
 Marvin Klusmeyer, Matrikelnummer: 5093298
*/


#define F_CPU 16000000
#include <avr/io.h>

void main()
{
	// OCR1A als ausgang setzen
	DDRB = 0xFF; 
	//NON Inverted PWM
	TCCR1A|=(1<<COM1A1)|(1<<COM1B1)|(1<<WGM11);  
	//PRESCALER=64 MODE 14(FAST PWM)
	TCCR1B|=(1<<WGM13)|(1<<WGM12)|(1<<CS11); 
	//fPWM=50Hz
	ICR1=40000;  
	//PWM Pins as Output
	DDRB|=0xFF;  
	//winkel festlegen 
	int angle = 180;
	//OCR1A einstellen
	OCR1A= 1000 + ((4100/180) * angle);  
	
	while(1){}
}
