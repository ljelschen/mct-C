#define F_CPU	16000000

#include <avr/io.h>
#include <avr/interrupt.h>

double analogWert = 0;

void startConversion(){
	// Startet die Auslesung
	ADCSRA |= (1 << ADSC); 
}
//Timer0 Overflow
ISR(TIMER0_OVF_vect){
	//OCR0A für die länge des PWM-Signal setzen (255 entsprechen 8 Bit)
	OCR0A = (analogWert/1024) * 255; 
}
//ADC fertig mit Auslesen
ISR(ADC_vect){ 
	// Analog Wert in einer Variable speichern
	analogWert = ADC;
	// ADC Wert auslesen
	startConversion(); 
}

int main(void){
	// Outputs setzen
	DDRD = 0xFF; 
	// OCR0A als ausgang setzten - PWM Mode Timer0
	TCCR0A = (1 << COM0A1) | (1 << WGM00) | (1 << WGM01); 
	// Timer0 Prescaler auf 1024
	TCCR0B = (1 << CS00) | (1 << CS02); 
	// Timer0 Interrupt aktivieren
	TIMSK0 = (1 << TOIE0); 
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
	while(1){}
}
