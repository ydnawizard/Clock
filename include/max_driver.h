#ifndef MAX7219_H_
#define MAX7219_H_

#include <stdint.h>
#include "stm32f4xx.h"

void SET_CS_LOW(void);
void SET_CS_HIGH(void);
void MAX_Chip_Send_Target(
		uint8_t target_chip, 
		uint8_t row_addr,
		uint8_t data
		);
void MAX_Chip_Send_All(uint8_t row_addr, uint8_t data);
void MAX_Init(void);
void MAX_Clear(void);






#endif //MAX7219_H_s
