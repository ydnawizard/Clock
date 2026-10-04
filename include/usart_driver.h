#ifndef USART_DRIVER_H_
#define USART_DRIVER_H_

#include "stm32f4xx.h"
#include <stddef.h>

static void usart_clock_en(USART_TypeDef * USARTx);
static void usart_clock_ds(USART_TypeDef * USARTx);
void usart_config(USART_TypeDef * USARTx, uint8_t mantissa, uint8_t fraction, uint8_t te, uint8_t re);
void usart_init();
void usart_init_targets();
void usart1_send_str(const char * string);
void usart1_send_char(char ch);
char usart1_read_char(void);

#endif //USART_DRIVER_H_
