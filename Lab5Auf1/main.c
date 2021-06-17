/*
 * Lab5Auf1.c
 *
 * Created: 2021-06-13 00:00:24
 * Author : Lars
 */ 

#define F_CPU	16000000

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/twi.h>
#include <util/delay.h>
//#include "DS3231.h"

#include "TWIPrimitives.h"

void TWIInit(void) {
    TWSR = 0x00; // Prescaler Value 0x00
    TWBR = 0x48; // TWI clock 100 kHz 0x48
    TWCR = (1<<TWEN); // Enable TWI
    DDRB = 1<<5;
}

void TWIStart(void) {
    PORTB = 0<<5;
    TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
    while (!(TWCR & (1<<TWINT)));
}

void TWIStop(void) {
    TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN); 
    while(TWCR & (1<<TWSTO));
}

void TWIWrite(uint8_t data) { 
    TWDR = data;
    TWCR = (1<<TWINT)|(1<<TWEN);
    while (!(TWCR & (1<<TWINT)));
}

uint8_t TWIReadACK(void) {
    TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
    while (!(TWCR & (1<<TWINT)));
    return TWDR;
}

uint8_t TWIReadNACK(void) {
    TWCR = (1<<TWINT)|(1<<TWEN);
    while (!(TWCR & (1<<TWINT)));
    return TWDR;
}

uint8_t TWIGetStatus(void) {
    uint8_t status;
    status = TWSR & 0xF8;
    return status;
}

void TWIError(void) {
    // Add your error routine here
    // e.g. turn red LED on
    PORTB = 1<<5;
    TWIStop();	
}


//registers
#define DS3231_ADDRESS       0xD0
#define DS3231_REG_SECONDS   0x00
#define DS3231_REG_AL1_SEC   0x07
#define DS3231_REG_AL2_MIN   0x0B
#define DS3231_REG_CONTROL   0x0E
#define DS3231_REG_STATUS    0x0F
#define DS3231_REG_TEMP_MSB  0x11

// converts BCD to decimal
uint8_t bcd_to_decimal(uint8_t number)
{
  return ( (number >> 4) * 10 + (number & 0x0F) );
}

// converts decimal to BCD
uint8_t decimal_to_bcd(uint8_t number)
{
  return ( ((number / 10) << 4) + (number % 10) );
}


void DS3231StartOsc(void) {
	TWIStart();
	if (TWIGetStatus() != 0x08) TWIError();
	
	TWIWrite((uint8_t)(DS3231_ADDRESS)); // Write operation
	if (TWIGetStatus() != 0x18) TWIError();
	
	TWIWrite((uint8_t)(0x0E)); // Control Register
	if (TWIGetStatus() != 0x28) TWIError();
	
	TWIWrite((uint8_t)(0x00)); // Enable
	if (TWIGetStatus() != 0x28) TWIError();
	
	TWIStop();
}



void DS3231SetTimeAndDate(uint8_t h, uint8_t m, uint8_t s, uint8_t dayOfWeek, uint8_t date, uint8_t month, unsigned int year) {

	year -= 2000; // Register has year value of (0-99)	
	
	s = decimal_to_bcd(s);
	m = decimal_to_bcd(m);
	h = decimal_to_bcd(h);
	date = decimal_to_bcd(date);
	month = decimal_to_bcd(month);
	year = decimal_to_bcd(year);
	
	//h = ((h + AMPM_MODE) << 6); // AMPM_MODE is "1" -> 12 hour mode

	uint8_t values[7] = {s, m, h, dayOfWeek, date, month, year};
	
	TWIStart();
	if (TWIGetStatus() != 0x08) TWIError();
	
	TWIWrite((uint8_t)(DS3231_ADDRESS)); // Write operation
	if (TWIGetStatus() != 0x18) TWIError();

	TWIWrite((uint8_t)(0x00)); // Start from seconds register (00h)
	if (TWIGetStatus() != 0x28) TWIError();

	uint8_t i = 0;
	for (i; i < 8; i++) { // Write 7 memory bytes (00h->07h)
		TWIWrite((uint8_t)(values[i]));  
		if (TWIGetStatus() != 0x28) TWIError();
	}	
	TWIStop();
}

uint8_t DS3231ReadDataFrom(uint8_t mem) {
	uint8_t u8data;
	TWIStart();
	if (TWIGetStatus() != 0x08) TWIError();
	TWIWrite((uint8_t)(DS3231_ADDRESS));
	if (TWIGetStatus() != 0x18) TWIError();
	TWIWrite((uint8_t)(mem));
	if (TWIGetStatus() != 0x28) TWIError();
	TWIStart();
	if (TWIGetStatus() != 0x10) TWIError();
	TWIWrite((uint8_t)(DS3231_ADDRESS + 1));
	if (TWIGetStatus() != 0x40) TWIError();
	u8data = TWIReadNACK();
	if (TWIGetStatus() != 0x58) TWIError();
	TWIStop();
	return u8data;
}

void DS3231WriteDataTo(uint8_t reg_address, uint8_t reg_value){
	TWIStart();
	if (TWIGetStatus() != 0x08) TWIError();

	TWIWrite((uint8_t)(DS3231_ADDRESS)); // Write operation
	if (TWIGetStatus() != 0x18) TWIError();

	TWIWrite((uint8_t)(reg_address)); // Start from seconds register (00h)
	if (TWIGetStatus() != 0x28) TWIError();

	TWIWrite((uint8_t)(reg_value)); // Start from seconds register (00h)
	if (TWIGetStatus() != 0x28) TWIError();

	TWIStop();
}

void EnableIntSqwt(void){
  uint8_t ctrl_reg = DS3231ReadDataFrom(DS3231_REG_CONTROL);
  ctrl_reg |= 0x04;
  DS3231WriteDataTo(DS3231_REG_CONTROL, ctrl_reg);
}

void DisableIntSqwt(void){
  uint8_t ctrl_reg = DS3231ReadDataFrom(DS3231_REG_CONTROL);
  ctrl_reg &= 0xFB;
  DS3231WriteDataTo(DS3231_REG_CONTROL, ctrl_reg);
}

uint8_t getBitFromRegister(uint8_t reg, uint8_t bit){
	uint8_t returnBit = 0; 

	if(bit == 0){
		returnBit = (reg & 0x01);
	}else if(bit ==  1){
		returnBit = (reg & 0x02)>>1;
	}else if(bit ==  2){
		returnBit = (reg & 0x04)>>2;
	}else if(bit ==  3){
		returnBit = (reg & 0x08)>>3;
	}else if(bit ==  4){
		returnBit = (reg & 0x10)>>3;
	}else if(bit ==  5){
		returnBit = (reg & 0x20)>>4;
	}else if(bit ==  6){
		returnBit = (reg & 0x40)>>5;
	}else if(bit ==  7){
		returnBit = (reg & 0x80)>>6;
	}
	return returnBit;
}

void SetAlarm1Rate(uint8_t alarm1Rate){
	
	uint8_t A1M1 = getBitFromRegister(alarm1Rate, 0) << 7;
	uint8_t A1M2 = getBitFromRegister(alarm1Rate, 1) << 7;
	uint8_t A1M3 = getBitFromRegister(alarm1Rate, 2) << 7;
	uint8_t A1M4 = getBitFromRegister(alarm1Rate, 3) << 7;


	TWIStart();
	if (TWIGetStatus() != 0x08) TWIError();

	TWIWrite((uint8_t)(DS3231_ADDRESS)); // Write operation
	if (TWIGetStatus() != 0x18) TWIError();

	TWIWrite((uint8_t)(DS3231_REG_AL1_SEC)); 
	if (TWIGetStatus() != 0x28) TWIError();

	TWIWrite((uint8_t)(A1M1));
	if (TWIGetStatus() != 0x28) TWIError();

	TWIWrite((uint8_t)(A1M2));
	if (TWIGetStatus() != 0x28) TWIError();

	TWIWrite((uint8_t)(A1M3));
	if (TWIGetStatus() != 0x28) TWIError();

	TWIWrite((uint8_t)(A1M4));
	if (TWIGetStatus() != 0x28) TWIError();

	TWIStop();
}

// enables alarm1
void EnableAlarm1(){
  uint8_t ctrl_reg = DS3231ReadDataFrom(DS3231_REG_CONTROL);
  ctrl_reg |= 0x01;
  DS3231WriteDataTo(DS3231_REG_CONTROL, ctrl_reg);
}

// disables alarm1
void DisableAlarm1(){
  uint8_t ctrl_reg = DS3231ReadDataFrom(DS3231_REG_CONTROL);
  ctrl_reg &= 0xFE;
  DS3231WriteDataTo(DS3231_REG_CONTROL, ctrl_reg);
}

// resets alarm1 flag bit
void ResetAlarm1(){
  uint8_t stat_reg = DS3231ReadDataFrom(DS3231_REG_STATUS);
  stat_reg &= 0xFE;
  DS3231WriteDataTo(DS3231_REG_STATUS, stat_reg);
}

void SetRS1kHz(){
  uint8_t stat_reg = DS3231ReadDataFrom(DS3231_REG_CONTROL);
  stat_reg |= 0x08;
  DS3231WriteDataTo(DS3231_REG_CONTROL, stat_reg);
}

uint8_t DS3231GetSeconds(void) {
	/*
	Aufgabe:
		Sekunden auslesen
	Zu Übergeben:
		-
	Rückgabe:
		uint8_t Sekunden in BCD (4BIT)

	Schritte:
		Wasser in Kaffeemaschine einfüllen,Filter in Kaffeemaschine einsetzen,
		Kaffeepulver in Filter einfüllen, Kaffeemaschine einschalten, Warten bis Kaffee durchgelaufen
	*/
	uint8_t u8data;	
	u8data = DS3231ReadDataFrom(0x00);
	u8data &= 0x0F;
	return u8data;
}
uint8_t DS3231GetMinutes(void) {
	uint8_t u8data;	
	u8data = DS3231ReadDataFrom(0x01);
	u8data &= 0x0F;
	return u8data;
}
uint8_t DS3231GetHours(void) {
	uint8_t u8data;	
	u8data = DS3231ReadDataFrom(0x02);
	u8data &= 0x0F;
	return u8data;
}
uint8_t DS3231GetDayOfWeek(void) {
	uint8_t u8data;	
	u8data = DS3231ReadDataFrom(0x03);
	return u8data;
}
uint8_t DS3231GetDate(void) {
	uint8_t u8data;	
	u8data = DS3231ReadDataFrom(0x04);
	u8data &= 0x0F;
	return u8data;
}
uint8_t DS3231GetMonth(void) {
	uint8_t u8data;	
	u8data = DS3231ReadDataFrom(0x05);
	u8data &= 0x0F;
	return u8data;
}
uint8_t DS3231GetYear(void) {
	uint8_t u8data;	
	u8data = DS3231ReadDataFrom(0x06);
	u8data &= 0x0F;
	return u8data;
}
uint8_t DS3231GetTenSeconds(void) {
	uint8_t u8data;	
	u8data = DS3231ReadDataFrom(0x00);
	u8data &= 0x70;
	u8data = (u8data >> 4);
	return u8data;
	
}
uint8_t DS3231GetTenMinutes(void) {
	uint8_t u8data;	
	u8data = DS3231ReadDataFrom(0x01);
	u8data = (u8data >> 4);
	return u8data;
}
uint8_t DS3231GetTenHours(void) {
	uint8_t u8data;	
	u8data = DS3231ReadDataFrom(0x02);
	u8data &= 0x30;
	u8data = (u8data >> 4);
	return u8data;
}
uint8_t DS3231GetTenDate(void) {
	uint8_t u8data;	
	u8data = DS3231ReadDataFrom(0x04);
	u8data &= 0x30;
	u8data = (u8data >> 4);
	return u8data;
}
uint8_t DS3231GetTenMonth(void) {
	uint8_t u8data;	
	u8data = DS3231ReadDataFrom(0x05);
	u8data = (u8data >> 4);
	return u8data;
}
uint8_t DS3231GetTenYear(void) {
	uint8_t u8data;	
	u8data = DS3231ReadDataFrom(0x06);
	u8data = (u8data >> 4);
	return u8data;
}

int Alarm1_Status()
{
  uint8_t ctrl_reg = DS3231ReadDataFrom(DS3231_REG_CONTROL);
  if(ctrl_reg & 0x01){
    PORTB = 1<<5;

  }else{
    PORTB = 0<<5;
  }
}


int zeahler = 0;

ISR(INT0_vect){

	//zeahler bis 60
	if(zeahler == 60){
		zeahler = 0; //setzt den zeahler zurueck
	}else{
		zeahler += 1; //erhoht den zeahler um 1
	}

	PORTC = getBitFromRegister(zeahler, 7); //setzt das 8.bit in PORTC
	PORTB = zeahler & 0x7F; //setzt die Restlichen bits in PORTB

	ResetAlarm1(); //setzt das interrupt in der RTC zurueck
}

int main(void){

	TWIInit(); //TWI-Bus init.
    DS3231StartOsc(); // aktivieren der RTC clock
	
	//festlegen der zeitvariablen
	uint8_t s = 12;
	uint8_t m = 12;
	uint8_t h = 12;
	uint8_t date = 1; // SUNDAY
	uint8_t month = 6; //JUNE
	uint8_t year = 21;
	uint8_t dayOfWeek = 7;
	DS3231SetTimeAndDate(h, m, s, dayOfWeek, date, month, year); //setzen der Zeit in der RTC

	SetAlarm1Rate(0x0F); //alarmrate auf sekuendlich setzen
	
	EnableAlarm1(); // Alarm 1 Interrupt aktivieren
	//EnableIntSqwt(); // Interrupts auf den Ausgang belegen

	DDRD = 0x00; // Port als Input
	DDRB = 0xFF; // Port als Ausgang
	DDRC = 0x01; //Port als Ausgang

	PORTD = 1<<2; // Pullup at Interruptpin
	EICRA = 1<<1; // INT0 at Falling edge
	EIMSK = 1<<0; //enable interrupt mask
	sei(); // global Interrupts einschalten

    while (1) {}
}
