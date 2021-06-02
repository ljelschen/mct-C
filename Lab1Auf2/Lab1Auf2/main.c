/*
 Titel: C Labor 1 Aufgabe 2
 Laborgruppe: A9
 
 Lars Jelschen, Matrikelnummer: 5100174
 Marvin Klusmeyer, Matrikelnummer: 5093298
*/


#define F_CPU 16000000UL

#include <avr/io.h>

#define LED1 0
#define LED2 1
#define LED3 2

#define Button1 3
#define Button2 4

int main(void){
	// Richtung der LED Als ausgang setzen
	DDRB = (1<<LED1) | (1<<LED2) | (1<<LED3); 
	//Pullups fuer die Buttons setzen 	
	PORTB = (1<<Button1)  | (1<<Button2);

	while (1){
		// Button Status
		int ButtonState = 0; 

		if(!(PINB & (1<<Button1))){
			// addiere wenn Button 1 ist gedrückt
			ButtonState += 1; 
		}

		if(!(PINB & (1<<Button2))){
			// addiere wenn Button 2 ist gedrückt
			ButtonState += 2;  
		}

		switch (ButtonState){
			case 1:
			// setze LED 1
				PORTB = (1<<LED1) | (0<<LED2) | (0<<LED3) | (1<<Button1)  | (1<<Button2); 
			break;
			case 2:
			// setze LED 2
				PORTB = (0<<LED1) | (1<<LED2) | (0<<LED3) | (1<<Button1)  | (1<<Button2); 
			break;
			case 3:
			// setze LED 3
				PORTB = (0<<LED1) | (0<<LED2) | (1<<LED3) | (1<<Button1)  | (1<<Button2);
			break;
			
			default:
			// alle LED's auf aus
				PORTB = (0<<LED1) | (0<<LED2) | (0<<LED3) | (1<<Button1)  | (1<<Button2);
			break;
		}
	}
}
