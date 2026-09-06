#ifndef RTC_DRIVER_
#define RTC_DRIVER_

#include "stm32f4xx.h"
#include <time.h>

void RTC_Init(void);
static uint8_t DecToBCD(uint8_t val);
static uint8_t BCDToDec(uint8_t val);
void RTC_SetTime(uint8_t hours, uint8_t minutes, uint8_t seconds);
void RTC_GetTime(uint8_t *hours, uint8_t *minutes, uint8_t *seconds);
void RTC_GetDate(uint8_t *year, uint8_t *month, uint8_t *day);
void RTC_GetStructTM(struct tm *out_time);
void RTC_SetDate(uint8_t year, uint8_t month, uint8_t day, uint8_t weekday);


#endif //RTC_DRIVER_
