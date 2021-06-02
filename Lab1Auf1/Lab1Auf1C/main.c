/*
 Titel: C Labor 1 Aufgabe 1
 Laborgruppe: A9
 
 Lars Jelschen, Matrikelnummer: 5100174
 Marvin Klusmeyer, Matrikelnummer: 5093298
*/


#include <avr/io.h>

//variablen definieren
#define LED1 0
#define LED2 1
#define LED3 2

int main(void){
    //Ausgangsrichtung setzten
    DDRB = 1<<LED1 | 1<<LED2 | 1<<LED3; 
    // Ausgaenge auf HIGH setzen
    PORTB = 1<<LED1 | 1<<LED2 | 1<<LED3; 

    while (1){}
}

