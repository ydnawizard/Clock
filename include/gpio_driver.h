
#ifndef GPIO_DRIVER_H_
#define GPIO_DRIVER_H_
#ifndef __COMPILER_BARRIER
  #define __COMPILER_BARRIER() __asm__ volatile("" ::: "memory")
#endif

#include "stm32f4xx.h"
#include <stdint.h>

void GPIO_Init(char port, uint8_t pin, uint8_t mode);
void GPIO_Disable(char port);
void GPIOA_Port_Number_Configure(uint8_t Pin_Number, uint8_t Mode_Cnf_Val);
void GPIOB_Port_Number_Configure(uint8_t Port_Number, uint8_t mode);
void GPIOC_Port_Number_Configure(uint8_t Port_Number, uint8_t mode);
void GPIOD_Port_Number_Configure(uint8_t Port_Number, uint8_t mode);
void GPIOE_Port_Number_Configure(uint8_t Port_Number, uint8_t mode);
void GPIOF_Port_Number_Configure(uint8_t Port_Number, uint8_t mode);
void GPIOG_Port_Number_Configure(uint8_t Port_Number, uint8_t mode);
void GPIOH_Port_Number_Configure(uint8_t Port_Number, uint8_t mode);
void GPIO_INIT_A567(void);
#endif
