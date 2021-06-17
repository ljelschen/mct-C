#include "DS3231.h"

TWIInit();

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


uint8_t DS3231GetSeconds(void) {
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