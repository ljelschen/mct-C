/*
 Titel: C Labor 3 Aufgabe 2
 Laborgruppe: A9
 
 Lars Jelschen, Matrikelnummer: 5100174
 Marvin Klusmeyer, Matrikelnummer: 5093298
*/


#include <avr/io.h>
#include <avr/interrupt.h>

//variablen für den Betrieb setzen
int betrieb = 0;
int lauflichtRichtung = 0;
int lauflichtPos = 0;

int zaehlerLauflicht = 0;
int zaehlerEinschalt = 0;
int zaehlerAusschalt = 0;

// Pinbelegung
#define Button 2
#define BetriebLED 3
#define LuefterLED 4


int toggle(void){
	// Button entprellen
	while((PINB & (1<<Button))){}
	
	//status der maschine aendern	
	if(betrieb == 0){
		// einschalt veriegelung
		if(!(PORTB & (1<<LuefterLED))){ 
			betrieb = 1;
			zaehlerEinschalt = 0;
			// Betrieb LED einschalten
			PORTB |= (1<<BetriebLED);	
		}
	}else{
		betrieb = 0; 
		zaehlerAusschalt = 0;
		// ausschalt Prozess wenn Lüfter noch aus
		if (!(PORTB & (1<<LuefterLED))){ 
			lauflichtPos = 0;
			PORTD = 0;
			PORTB &= ~(1<<0);
			PORTB &= ~(1<<1);
			PORTB &= ~(1<<BetriebLED);
		}
	}
		
	return 1;
}

int lauflicht(void){
	// PORT's auf 0 setzen
	PORTD = 0;
	PORTB &= ~(1<<0);
	PORTB &= ~(1<<1);
	
	// Lauflicht setzen
	switch (lauflichtPos){
		case 0:
			PORTD = 1<<2;
			// lauflicht Richtung wechseln
			lauflichtRichtung = 0; 
		break;

		case 1:
			PORTD = 1<<3;
		break;
		
		case 2:
			PORTD = 1<<4;
		break;

		case 3:
			PORTD = 1<<5;
		break;

		case 4:
			PORTD = 1<<6;
		break;

		case 5:
			PORTD = 1<<7;
		break;

		case 6:
			PORTB |= (1<<0);
		break;

		case 7:
			PORTB |= (1<<1);
			// lauflicht Richtung wechseln
			lauflichtRichtung = 1; 
		break;
	}
	
	// Lauflicht position verändern
	if(lauflichtRichtung){
		// runter Laufen
		lauflichtPos--; 
	}else{
		// hoch Laufen
		lauflichtPos++; 
	}

	return 1;
}

// Timer Interrupt
ISR( TIMER0_OVF_vect ) { 
	// nach 0.2s
	if(zaehlerLauflicht == 12){ 
	// aktiviere Lauflicht, nur wenn betrieb an ist
		if (betrieb == 1){ 
			lauflicht(); 
		}
		// Zähler zurueck setzen
		zaehlerLauflicht = 0;		
	}

	// nach 2s 
	if (zaehlerEinschalt == 122 && betrieb == 1){
		// Luefter LED anschalten, wenn sie noch nocht eingeschaltet sind
		if (!(PORTB & (1<<LuefterLED))){
			PORTB |= (1<<LuefterLED);
		}
	}
	
	// nach 4s
	if (zaehlerAusschalt == 244 && betrieb == 0){
		// setze alle Ports und zähler zurück
		lauflichtPos = 0;
		PORTD = 0;
		PORTB &= ~(1<<0);
		PORTB &= ~(1<<1);
		PORTB &= ~(1<<BetriebLED);
		PORTB &= ~(1<<LuefterLED);
	}
	
	// zähler um ein erhoehen
	zaehlerLauflicht++;
	zaehlerEinschalt++;
	zaehlerAusschalt++;
	sei();// global interrupt ein
}


int main(void)
{
	// Ausgaenge in PORTD setzen
    DDRD = 0xFF;
    PORTD = 0x00;

	// Ausgaenge und Eingaege setzen
    DDRB = 0<<Button | 1<<BetriebLED | 1<<LuefterLED | 1<<0 | 1<<1;
	// Pull-ups setzten
    PORTB = 1<<Button | 0<<BetriebLED | 0<<LuefterLED;

	// Timer0 init. 
    TCCR0B = 1<<CS00 | 1<<CS02;
    // Timer 0 interrupt einschalten
	TIMSK0 = 1<<TOIE0;
	// global interrupt einschalten
	sei();

    /* Replace with your application code */
    while (1) 
    {
		// Button druck ueberpuefen
        if((PINB & (1<<Button))){
            toggle();
		}
    }
}


