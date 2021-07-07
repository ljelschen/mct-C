#ifndef DS3231
#define DS3231

#include "TWIPrimitives.h"

uint8_t bcd_to_decimal(uint8_t number);
uint8_t decimal_to_bcd(uint8_t number);

void DS3231StartOsc(void);
void DS3231SetTimeAndDate(uint8_t h, uint8_t m, uint8_t s
, uint8_t dayOfWeek, uint8_t date, uint8_t month, unsigned int year);

uint8_t DS3231ReadDataFrom(uint8_t mem);

uint8_t DS3231GetSeconds(void);
uint8_t DS3231GetMinutes(void);
uint8_t DS3231GetHours(void);
uint8_t DS3231GetDayOfWeek(void);
uint8_t DS3231GetDate(void);
uint8_t DS3231GetMonth(void);
uint8_t DS3231GetYear(void);

uint8_t DS3231GetTenSeconds(void);
uint8_t DS3231GetTenMinutes(void);
uint8_t DS3231GetTenHours(void);
uint8_t DS3231GetTenDate(void);
uint8_t DS3231GetTenMonth(void);
uint8_t DS3231GetTenYear(void);

#endif
