#define F_CPU	16000000

#include <avr/io.h>
#include <avr/interrupt.h>

double analogWert = 0;

void startConversion(){
	// Startet die Auslesung
	ADCSRA |= (1 << ADSC); 
}
//ADC fertig mit Auslesen
ISR(ADC_vect){ 
	//global interrupt an
	sei(); 
	// analogwert in Variable speichern
	analogWert = ADC; 
 //Analogwert umwandeln
  int led = (analogWert/1024) * 11;

	if (led <= 1){
		PORTB = 0;
		PORTD = 1<<2; //1 - LED
	}else if (led <= 2){
		PORTB = 0;
		PORTD = 1<<3; //2 - LED
	}else if (led <= 3){
		PORTB = 0;
		PORTD = 1<<4; //3 - LED
	}else if (led <= 4){
		PORTB = 0;
		PORTD = 1<<5; //4 - LED
	}else if (led <= 5){
		PORTB = 0;
		PORTD = 1<<6; //5 - LED
	}else if (led <= 6){
		PORTB = 0;
		PORTD = 1<<7; //6 - LED

    //-------- Port wechsel von PORTD auf PORTB --------
	}else if (led <= 7){   
		PORTD = 0;
		PORTB = 1<<0; //7 - LED
	}else if (led <= 8){
		PORTD = 0;
		PORTB = 1<<1; //8 - LED
	}else if (led <= 9){
		PORTD = 0;
		PORTB = 1<<2; //9 - LED
	}else if (led <= 10){
		PORTD = 0;
		PORTB = 1<<3; //10 - LED
	}	
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
	// Enable - ADC Interrupt - ADC Prescaler auf 4 (Was macht der Prescaler ?)
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
