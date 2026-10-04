#include <gpio_driver.h>
#include <usart_driver.h>
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
		gpio_init_targets();
		usart_init_targets();
		SET_CS_HIGH();
		SET_CS_LOW();
		MAX_Init();
		RTC_Init();
		RTC_SetTime(18,35,0);
		RTC_SetDate(2026,9,5,1);
		//INIT DISPLAY
		struct dis cur_dis;
		struct dis * _dis = &cur_dis;
		//INIT ANIMATION
		struct ani time_ani;
		ani_init(&time_ani);
		struct ani_seq cur_ani_seq;
		cur_ani_seq.proc = malloc(1 * sizeof(ani));
		cur_ani_seq.proc[0]  = time_ani;
		_dis->_ani_seq = &cur_ani_seq;
		//INIT TIME
		struct tm current_time;
		time_t t;
		t = time(NULL);
		_dis->_time = localtime(&t);
		//Copy time into str_time
		char str_time[15];
		uint8_t str_len,y,m,d;
		str_len = 21;
		RTC_GetDate(&y,&m,&d);
		ani_str_set(&_dis->_ani_seq->proc[0],"1O:O5 am CST Sat Oct O3 2O26");
		usart1_send_str("AT+PIO11\r\n");
		ani_str_set(&_dis->_ani_seq->proc[0],(char *)usart1_read_char());
		while(1)
		{
			scroll_horizontal(&_dis->_ani_seq->proc[0]);
			//ani_str_set(&_dis->_ani_seq->proc[0],"International");
		}
}



void _init(void){}
