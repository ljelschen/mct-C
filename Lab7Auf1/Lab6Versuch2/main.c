#define F_CPU 16000000UL
#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <string.h>

#include "uart.h"
#include "uart.c"

#include "DHT.h"
#include "DHT.c"

#include "lcd-routines.c"
#include "lcd-routines.h"

#include "TWIPrimitives.h"
#include "TWIPrimitives.c"

#include "DS3231.h"
#include "DS3231.c"

#define UART_BAUD_RATE      9600      

//Variablen
char buffer[7];
unsigned int c;
char bufferTemp[10];
char bufferHum[10];
int delay = 0;
int lastSec = 0;

char input[28];
int inputPos = 0;
int inputPair = 0;

int setTimeAndDate(void){

    lcd_setcursor(0, 1);
    //------- Datum auf dem LCD -------
    itoa(bcd_to_decimal(DS3231GetTenDate()) , buffer, 10);
	lcd_string(buffer);
	itoa(bcd_to_decimal(DS3231GetDate()) , buffer, 10);
	lcd_string(buffer);
	lcd_string(".");

    // Monate werden umgewandelt in Texte
    int month = bcd_to_decimal(DS3231GetMonth());
    int tenMonth = bcd_to_decimal(DS3231GetTenMonth());
	if(tenMonth == 0 && month == 1 ){
        lcd_string("Jan");
    }else if( tenMonth == 0 && month == 2 ) {
        lcd_string("Feb");
    }else if( tenMonth == 0 && month == 3 ) {
        lcd_string("Mar");
    }else if( tenMonth == 0 && month == 4 ) {
        lcd_string("Apr");
    }else if( tenMonth == 0 && month == 5 ) {
        lcd_string("May");
    }else if( tenMonth == 0 && month == 6 ) {
        lcd_string("Jun");
    }else if( tenMonth == 0 && month == 7 ) {
        lcd_string("Jul");
    }else if( tenMonth == 0 && month == 8 ) {
        lcd_string("Aug");
    }else if( tenMonth == 0 && month == 9 ) {
        lcd_string("Sep");
    }else if( tenMonth == 1 && month == 0 ) {
        lcd_string("Oct");
    }else if( tenMonth == 1 && month == 1 ) {
        lcd_string("Nov");
    }else if( tenMonth == 1 && month == 2 ) {
        lcd_string("Dec");
    }
    lcd_string(". ");

    //------- Zeit -------
	itoa(bcd_to_decimal(DS3231GetTenHours()) , buffer, 10);
	lcd_string(buffer);
	itoa(bcd_to_decimal(DS3231GetHours()) , buffer, 10);
	lcd_string(buffer);
	lcd_string(":");
	itoa(bcd_to_decimal(DS3231GetTenMinutes()) , buffer, 10);
	lcd_string(buffer);
	itoa(bcd_to_decimal(DS3231GetMinutes()) , buffer, 10);
	lcd_string(buffer);
    lcd_string(":");
    itoa(bcd_to_decimal(DS3231GetTenSeconds()) , buffer, 10);
	lcd_string(buffer);
    int sec = bcd_to_decimal(DS3231GetSeconds());
	itoa(sec, buffer, 10);
	lcd_string(buffer);

    //zeitverzoegerung für den button
    if (delay > 0 && lastSec != sec){
        delay -= 1;
        lastSec = sec;
    }
}


int setTempAndHum(void){

    lcd_setcursor(0,2);
    //------- Temperatur -------
    double temp = 0;
    // solange auslesen, bis daten vorhanden sind
    while(temp == 0){
        DHT_GetTemperature(&temp);
        _delay_ms(100);
    }
    itoa( temp, bufferTemp, 10); //umwandeln in char   
    lcd_string("Th: ");
    //schreiben der einzelnen chars
    lcd_data(bufferTemp[0]);
    lcd_data(bufferTemp[1]);

    //------- Luftfeuchtigkeit -------
    lcd_data((char)223); // grad Zeichen
    lcd_string("C Hu: ");
    double humidity = 0;
    // solange auslesen, bis daten vorhanden sind
    while(humidity == 0){
        DHT_GetHumidity(&humidity);
        _delay_ms(100);
    }
    itoa( humidity, bufferHum, 10); //umwandeln in char
    //schreiben der einzelnen chars
    lcd_data(bufferHum[0]); 
    lcd_data(bufferHum[1]);
    lcd_string("%");
}

int workInput(void){
    // eingabe ausgaben
    uart_puts("\nErkannte Eingabe:\n");
    uart_puts(input);

    int h, m, s, date, month, year, temp, hum;
    //text erkennung anhand der sscanf funktion
    sscanf(input, "%d-%d-%dT%d:%d:%dTh%dHu%d", &year, &month, &date, &h, &m, &s, &temp, &hum);
    // zuruecksetzen der input variable
    memset(input, 0, 28);
    inputPos = 0; // input pos auf 0

    // dayOfWeek wird nichts genutzt deswegen belibig gesetzt
    uint8_t dayOfWeek = 7;
    DS3231SetTimeAndDate(h, m, s, dayOfWeek, date, month, year); //setzen der zeit in der RTC
    
    //schreiben der Temp. auf LCD
    lcd_setcursor(4,2); //cursor an stelle der Temp.
    //umwandeln von int. in char
    itoa( temp, bufferTemp, 10); 
    lcd_data(bufferTemp[0]);
    lcd_data(bufferTemp[1]);

    //schreiben von der Luftfeu. auf LCD
    lcd_setcursor(13,2);  //cursor an stelle der Luftfeu.
    //umwandeln von int. in char
    itoa( hum, bufferHum, 10);
    lcd_data(bufferHum[0]);
    lcd_data(bufferHum[1]);
}

// Minuten Alarm Interrupt
ISR(INT0_vect){
    //setzen der Temp. und Luft.
	setTempAndHum(); 
    //Alarm zuruecksetzen
    ResetAlarm2();
}

int main(void)
{
    lcd_init();	 // display init
    uart_init( UART_BAUD_SELECT(UART_BAUD_RATE,F_CPU) ); //bus init
    
    
    DS3231StartOsc();
	//festlegen der zeitvariablen
	uint8_t s = 50;
	uint8_t m = 59;
	uint8_t h = 23;
	uint8_t date = 31; // SUNDAY
	uint8_t month = 12; //JUNE
	int year = 2021;
	uint8_t dayOfWeek = 7;
	DS3231SetTimeAndDate(h, m, s, dayOfWeek, date, month, year); //setzen der Zeit in der RTC
  
	SetAlarm2Rate(0x0F); //alarmrate auf minutlich setzen
	EnableAlarm2(); // Alarm 2 Interrupt aktivieren
	EnableIntSqwt(); // Interrupts auf den Ausgang belegen
    
    DDRD = 0<<2 | 0<<3; // Eingaenge setzen
	PORTD = 1<<2 | 1<<3; // Pullup at Interruptpin
	EICRA = 1<<1; // INT0 and INT1 at Falling edge
	EIMSK = 1<<0; //enable interrupt mask
	sei(); // global Interrupts einschalten
    
    uart_puts("Verbunden und bereit für die Eingabe:\n"); //schreibt in dem Bus 
    uart_puts("Eingabeformat -> (YYYY-MM-DDThh:mm:iiThXXHuYY) + CR:\n");
    uart_puts("----------------------------------------------------\n");
    uart_putc('\r');

    //zeit und temp setzen
    setTimeAndDate();
    setTempAndHum();

    for(;;){    
        c = uart_getc(); // uart bus lesen
      
        // wenn es keine uart daten verfügbar
        if ( c & UART_NO_DATA ){
            setTimeAndDate();  //zeit setzen

            // Button druck abfragen mit der verzoegerung
            if(!(PIND & (1<<3)) && delay == 0){
                setTempAndHum(); //temp auslesen und setzen
                delay = 2; //delay auf 2 sec.
            }
            _delay_ms(100); // kleiner delay
        }else{
            //pruefen nach eine zeichenkette länge von 27 Zeichen = 2021-07-01T12:23:00Th35Hu35
            if(strlen(input) == 27){
                workInput(); //eingabe verarbeiten
            }else{
                //zeichenkette zusammenbauen
                input[inputPos] = c;
                //zeichenkette position erhöhen
                inputPos += 1;
            }
        }
    }
    
}
