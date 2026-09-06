#include <gpio_driver.h>
#include <max_driver.h>
#include <rtc_driver.h>
#include <animation.h>
#include "stm32f4xx.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <display.h>

int main(void)
{
		//INIT DRIVERS
		GPIO_INIT_A567();
		SET_CS_HIGH();
		SET_CS_LOW();
		MAX_Init();
		RTC_Init();
		RTC_SetTime(18,35,0);
		RTC_SetDate(26,9,5,6);
		//INIT DISPLAY TIME ANIMATION
		struct display current_display;
		struct animation current_animation;
		struct display * _display = &current_display;
		struct tm current_time;
		time_t t;
		t = time(NULL);
		_display->_time = localtime(&t);
		//Copy time into str_time
		char str_time[8] = "        ";
		int str_len = 8;
		uint8_t h,m,s;
		RTC_GetTime(&h,&m,&s);
		RTC_GetStructTM(_display->_time);
		strncpy(str_time,asctime(_display->_time),str_len);
		//STORE INDIVIDUAL CHARS AS A BIT ARRAYS 
		uint8_t ** bit8_str = malloc(str_len * sizeof(uint8_t *));
		//STORE CHARS AS 64 BIT STRING ARRAY
		uint64_t * bit64_str = calloc(64, 8 * sizeof(uint64_t));
		//FETCH CHARS AND CONCAT 
		String_Fetch(str_time, &bit8_str,str_len); 
		Cat_String_864(bit8_str, bit64_str, str_len);
		while(1)
		{
			Recursive_Scroll_Horizontal(&bit64_str, str_len, 0, 4, 1);
		}
}



void _init(void){}
