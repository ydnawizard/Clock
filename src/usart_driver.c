#include "usart_driver.h"


static void usart_clock_en(USART_TypeDef * USARTx)
{
	if (USARTx == USART1) RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	else if (USARTx == USART6) RCC->APB2ENR |= RCC_APB2ENR_USART6EN;
}

static void usart_clock_ds(USART_TypeDef * USARTx)
{
	if (USARTx == USART1) RCC->APB2ENR |= ~RCC_APB2ENR_USART1EN;
	else if (USARTx == USART6) RCC->APB2ENR |= ~RCC_APB2ENR_USART6EN;
}

void  usart_config(
		USART_TypeDef * USARTx,
		uint8_t mantissa,
		uint8_t fraction,
		uint8_t te,
		uint8_t re
		)
{
	usart_clock_en(USARTx);
	USARTx->BRR = (mantissa << 4) | fraction;
	if(te)
	{
		USARTx->CR1 |= USART_CR1_TE;
	}
	if(re)
	{
		USARTx->CR1 |= USART_CR1_RE;
	}
	USARTx->CR1 |= USART_CR1_UE;
}
// Send a single character
void usart1_send_char(char ch) {
    // Wait until Transmit Data Register is empty (TXE bit)
    while (!(USART1->SR & USART_SR_TXE));
    
    USART1->DR = (ch & 0xFF);
}

// Receive a single character (Blocking)
char usart1_read_char(void) {
    // Wait until Read Data Register is Not Empty (RXNE bit)
    while (!(USART1->SR & USART_SR_RXNE));
    
    return (char)(USART1->DR & 0xFF);
}

// Helper to send strings
void usart1_send_str(const char *str) {
    while (*str) {
        usart1_send_char(*str++);
    }
}

void usart_init_targets()
{
	usart_config(USART1, 104, 3, 1, 1);
}
