/*
 * Lab6Versuch2.c
 *
 * Created: 20.06.2021 00:51:46
 * Author : Lars
 */ 
#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/twi.h>
#include <util/delay.h>
#include <stdlib.h>

#include "TWIPrimitives.h"
#include "TWIPrimitives.c"

#include "DS3231.h"
#include "DS3231.c"

#include "lcd-routines.c"
#include "lcd-routines.h"

char buffer[7];

void settime(void){
	//Time
	itoa(bcd_to_decimal(DS3231GetSeconds()) , buffer, 10);
	lcd_setcursor(7, 2);
	lcd_string(buffer);
	
	itoa(bcd_to_decimal(DS3231GetTenSeconds()) , buffer, 10);
	lcd_setcursor(6, 2);
	lcd_string(buffer);
	
	lcd_setcursor(5, 2);
	lcd_string(":");
	
	itoa(bcd_to_decimal(DS3231GetMinutes()) , buffer, 10);
	lcd_setcursor(4, 2);
	lcd_string(buffer);
	
	itoa(bcd_to_decimal(DS3231GetTenMinutes()) , buffer, 10);
	lcd_setcursor(3, 2);
	lcd_string(buffer);
	
	lcd_setcursor(2, 2);
	lcd_string(":");
	
	itoa(bcd_to_decimal(DS3231GetHours()) , buffer, 10);
	lcd_setcursor(1, 2);
	lcd_string(buffer);
	
	itoa(bcd_to_decimal(DS3231GetTenHours()) , buffer, 10);
	lcd_setcursor(0, 2);
	lcd_string(buffer);

	//Date
	itoa(bcd_to_decimal(DS3231GetDate()) , buffer, 10);
	lcd_setcursor(1, 1);
	lcd_string(buffer);
	
	itoa(bcd_to_decimal(DS3231GetTenDate()) , buffer, 10);
	lcd_setcursor(0, 1);
	lcd_string(buffer);
	
	lcd_setcursor(2, 1);
	lcd_string(".");
	
	itoa(bcd_to_decimal(DS3231GetMonth()) , buffer, 10);
	lcd_setcursor(4, 1);
	lcd_string(buffer);
	
	itoa(bcd_to_decimal(DS3231GetTenMonth()) , buffer, 10);
	lcd_setcursor(3, 1);
	lcd_string(buffer);
	
	lcd_setcursor(5, 1);
	lcd_string(".");
	
	itoa(bcd_to_decimal(DS3231GetYear()) , buffer, 10);
	lcd_setcursor(7, 1);
	lcd_string(buffer);
	
	itoa(bcd_to_decimal(DS3231GetTenYear()) , buffer, 10);
	lcd_setcursor(6, 1);
	lcd_string(buffer);
	//DayofWeek
	int dayOfWeek = bcd_to_decimal(DS3231GetDayOfWeek());
	itoa (dayOfWeek , buffer, 10);
	lcd_setcursor(13, 2);
	if (dayOfWeek == 0){
		lcd_string("Sun");
	} else if (dayOfWeek == 1){
		lcd_string("Mon");
	} else if (dayOfWeek == 2){
		lcd_string("Tue");
	} else if (dayOfWeek == 3){
		lcd_string("Wed");
	} else if (dayOfWeek == 4){
		lcd_string("Thu");
	} else if (dayOfWeek == 5){
		lcd_string("Fri");
	} else if (dayOfWeek == 6){
		lcd_string("Sat");
	}
}

int alarm = 0;
ISR(INT0_vect){
	alarm = 1;
}

ISR(INT1_vect){
	alarm = 0;
	ResetAlarm2(); //setzt das interrupt in der RTC zurueck
}


int main(void){
	DDRD = 0x00; // Port als Input
	lcd_init();	
	TWIInit(); //TWI-Bus init.
	DS3231StartOsc();

	//festlegen der zeitvariablen
	uint8_t s = 50;
	uint8_t m = 59;
	uint8_t h = 23;
	uint8_t date = 31; // SUNDAY
	uint8_t month = 12; //JUNE
	uint8_t year = 2021;
	uint8_t dayOfWeek = 7;
	DS3231SetTimeAndDate(h, m, s, dayOfWeek, date, month, year); //setzen der Zeit in der RTC
  
	SetAlarm2Rate(0x0F); //alarmrate auf minutlich setzen
	EnableAlarm2(); // Alarm 2 Interrupt aktivieren
	EnableIntSqwt(); // Interrupts auf den Ausgang belegen

	PORTD = 1<<2 | 1<<3; // Pullup at Interruptpin
	EICRA = 1<<1 | 1<<3; // INT0 and INT1 at Falling edge
	EIMSK = 1<<0 | 1<<1; //enable interrupt mask
	sei(); // global Interrupts einschalten

    while(1) 
    {
		if(alarm == 1){
			lcd_clear();
			lcd_setcursor(0, 1);
			lcd_string("Alarm");
		}else{
			settime();
		}
	
		_delay_ms(500);
    }
}

