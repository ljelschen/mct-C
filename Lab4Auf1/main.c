#define F_CPU	16000000

#include <avr/io.h>
#include <avr/interrupt.h>

uint16_t analogWert = 0;

void startConversion(){
	// Startet die Auslesung
	ADCSRA |= (1 << ADSC); 
}
//ADC fertig mit Auslesen
ISR(ADC_vect){ 
	// analogwert in Variable speichern
	analogWert = ADC; 
	
	//um zwei stellen Verschieben 
	// 11 11111111 -> 1111 11111100
	uint16_t value = analogWert<<2;

	//oberen und unteren bytes trennen
	// obere:  0000 1111
	uint8_t high = (value >> 8);
	// untere: 1111 1100
	uint8_t low = (value & 0x00FF);
	//bytes die Ausgänge zuweisen
	PORTB = high;
	PORTD = low;
	// ADC Wert auslesen
	startConversion(); 
}

int main(void){
  //Richtung der Ausgänge setzen
	DDRD = 0xFF;
	DDRB = 0xFF;
 //ADC init.
  //(A5) und externe REF - Spannung
	ADMUX = (1 << REFS0) | (1 << MUX0) | (1 << MUX2); 
	// Enable - ADC Interrupt - ADC Prescaler auf 4
	ADCSRA = (1 << ADEN) | (1 << ADIE) | (1 << ADPS1);
	// Input aktivieren (A5)
	DIDR0 = (1 << ADC5D);
	// ADC Wert auslesen
	startConversion();
	//global interrupt an
	sei(); 
	// nichts tun
	while(1){} 
}
