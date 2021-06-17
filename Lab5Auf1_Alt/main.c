#define F_CPU	16000000

#include <avr/io.h>
#include <avr/interrupt.h>
#include "DS3231.h"

ISR(TIMER1_COMPA_vect){

}


int main(void){
    DS3231StartOsc();
	uint8_t s = 12;
	uint8_t m = 12;
	uint8_t h = 12;
	uint8_t date = 1; // SUNDAY
	uint8_t month = 6; //JUNE
	uint8_t year = 21;
	uint8_t dayOfWeek = 7;
	DS3231SetTimeAndDate(h, m, s, dayOfWeek, date, month, year);
	while(1){
	}
}
