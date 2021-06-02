#define F_CPU	16000000

#include <avr/io.h>
#include <avr/interrupt.h>

double analogWert = 0;
int angle = 0;

void startConversion(){
	// Startet die Auslesung
	ADCSRA |= (1 << ADSC); 
}
 //Timer1 Overflow
ISR(TIMER1_COMPA_vect){
	// global interrupts enable
	sei(); 
	//umrechen von Analog Wert in Grad
	angle = (analogWert/1024)  * 180; 
	// einstellen von PWM-Signal mit Grad
	OCR1A= 1000 + ((4100/180) * angle); 
}
//ADC fertig mit Auslesen
ISR(ADC_vect){ 
	// global interrupts enable
	sei(); 
	// analogwert in Variable speichern
	analogWert = ADC; 
}

int main(void){
	//OCR1A als Ausgang setzen - D8 als Input setzen
	DDRB = (1<<1) | (0<<0); 
	// Interrupt für D8 Aktivieren
	PORTB = (1<<0); 
	//Timer1 Einstellungen
	 //non-inverting PWM 
	TCCR1A|=(1<<COM1A1)|(1<<COM1B1)|(1<<WGM11); 
	 //PRESCALER=64 MODE 14(FAST PWM)
	TCCR1B|=(1<<WGM13)|(1<<WGM12)|(1<<CS11);
	 //fPWM=50Hz
	ICR1=40000; 
	 //PWM Pins as Output
	DDRB|=0xFF;  
	//0 degree (Startwert setzen)
	OCR1A= 2000 + ((2000/180) * angle);  
	//interrupt OCR1A setzen
	TIMSK1 = (1<<OCIE1A); 
	
	// ADC einstellungen
	//(A5) und externe REF - Spannung
	ADMUX = (1 << REFS0) | (1 << MUX0) | (1 << MUX2); 
	// Enable - ADC Interrupt - ADC Prescaler auf 4 (Was macht der Prescaler ?)
	ADCSRA = (1 << ADEN) | (1 << ADIE) | (1 << ADPS1); 
	// Input aktivieren (A5)
	DIDR0 = (1 << ADC5D); 
	// global interrupts enable
	sei(); 
		
	while(1){
		//Pin D8 als Button-Pin
		if(!(PINB & (1<<0))){ 
			// ADC Wert auslesen
            startConversion(); 
		}
	}
}
