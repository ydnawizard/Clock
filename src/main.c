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
		RTC_SetDate(2026,9,5,1);
		//INIT DISPLAY
		struct display current_display;
		struct display * _display = &current_display;
		//INIT ANIMATION
		struct animation time_animation;
		time_animation.effect_count = 1;
		time_animation.font = 0;
		time_animation.string = malloc(sizeof("17:O2 Tue Sep 22 2O26"));
		time_animation.string = "17:O2 Tue Sep 22 2O26";
		struct animation_sequence current_animation_sequence;
		current_animation_sequence.procession = malloc(1 * sizeof(animation));
		current_animation_sequence.procession[0]  = time_animation;
		_display->_animation_sequence = &current_animation_sequence;
		//INIT TIME
		struct tm current_time;
		time_t t;
		t = time(NULL);
		_display->_time = localtime(&t);
		//Copy time into str_time
		char str_time[15];
		uint8_t str_len,y,m,d;
		str_len = 21;
		RTC_GetDate(&y,&m,&d);
		//STORE INDIVIDUAL CHARS AS A BIT ARRAYS 
		uint8_t ** bit8_str;
		//FETCH CHARS FROM FONT DICT
		string_fetch(_display->_animation_sequence->procession[0].string, &bit8_str,str_len); 
		while(1)
		{
			recursive_scroll_horizontal(bit8_str, str_len, 0, 3, 1);
		}
}



void _init(void){}
