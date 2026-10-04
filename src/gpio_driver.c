#include "gpio_driver.h"
static void gpio_clock_en(GPIO_TypeDef *GPIOx)
{
    if (GPIOx == GPIOA)      RCC->AHB1ENR |= GPIOAEN;
    else if (GPIOx == GPIOB) RCC->AHB1ENR |= GPIOBEN;
    else if (GPIOx == GPIOC) RCC->AHB1ENR |= GPIOCEN;
    else if (GPIOx == GPIOD) RCC->AHB1ENR |= GPIODEN;
    else if (GPIOx == GPIOE) RCC->AHB1ENR |= GPIOEEN;
    else if (GPIOx == GPIOH) RCC->AHB1ENR |= GPIOHEN;
}

static void gpio_clock_ds(GPIO_TypeDef *GPIOx)
{
    if (GPIOx == GPIOA)      RCC->AHB1ENR &= ~GPIOAEN;
    else if (GPIOx == GPIOB) RCC->AHB1ENR &= ~GPIOBEN;
    else if (GPIOx == GPIOC) RCC->AHB1ENR &= ~GPIOCEN;
    else if (GPIOx == GPIOD) RCC->AHB1ENR &= ~GPIODEN;
    else if (GPIOx == GPIOE) RCC->AHB1ENR &= ~GPIOEEN;
    else if (GPIOx == GPIOH) RCC->AHB1ENR &= ~GPIOHEN;
}

void gpio_pin_cfg(GPIO_TypeDef *GPIOx, uint8_t pin, uint8_t speed, uint8_t mode)
{
    if (pin > 15) return;

    //Ensure clock for the peripheral is enabled
    gpio_clock_en(GPIOx);

    //Configure Pin Mode in MODER (2 bits per pin)
    uint8_t shift = pin * 2;
    GPIOx->MODER &= ~(0x3U << shift);          // Clear 2-bit mode field
    GPIOx->MODER |= ((uint32_t)mode << shift);  // Set new mode value
    GPIOx->OSPEEDR &= ~(0x3U << shift);
    GPIOx->OSPEEDR |= (((uint32_t)speed & 0x3U) << shift);
   
}

void gpio_pin_alt_cfg(GPIO_TypeDef * GPIOx, uint8_t pin, uint8_t function)
{
	if(pin == 0)
	{
		GPIOx->AFR[1] &= ~(0xF << GPIO_AFRL_AFSEL0_Pos);
		GPIOx->AFR[1] |= (function << GPIO_AFRL_AFSEL0_Pos);
	}
	else if(pin == 1)
	{
		GPIOx->AFR[1] &= ~(0xF << GPIO_AFRL_AFSEL1_Pos);
		GPIOx->AFR[1] |= (function << GPIO_AFRL_AFSEL1_Pos);
	}
}

static GPIO_TypeDef * gpio_get_port(char port)
{
    switch(port) {
        case 'a': case 'A': return GPIOA;
        case 'b': case 'B': return GPIOB;
        case 'c': case 'C': return GPIOC;
        case 'd': case 'D': return GPIOD;
        case 'e': case 'E': return GPIOE;
        case 'h': case 'H': return GPIOH;
        default: return NULL;
    }
}

// Configures pin mode given a port char ('a'-'h'), pin number (0-15), and mode (0-3)
void gpio_init(char port, uint8_t pin_number, uint8_t speed, uint8_t mode)
{
    GPIO_TypeDef *GPIOx = gpio_get_port(port);
    if (GPIOx != NULL) {
        gpio_pin_cfg(GPIOx, pin_number, speed, mode);
    }
}

// Disables peripheral clock for a port
void gpio_ds(char port)
{
    GPIO_TypeDef *GPIOx = gpio_get_port(port);
    if (GPIOx != NULL) {
        gpio_clock_ds(GPIOx);
    }
}

//Initialize gpio ports used for SPI and USART
void gpio_init_targets(void)
{
    gpio_pin_cfg(GPIOA, 5,3, GPIO_MODE_OUTPUT);
    gpio_pin_cfg(GPIOA, 6,3, GPIO_MODE_OUTPUT);
    gpio_pin_cfg(GPIOA, 7,3, GPIO_MODE_OUTPUT);
    gpio_pin_cfg(GPIOA, 0,3, GPIO_MODE_ALT);
    gpio_pin_cfg(GPIOA, 1,3, GPIO_MODE_ALT);
    gpio_pin_alt_cfg(GPIOA,0,7);
    gpio_pin_alt_cfg(GPIOA,1,7);
}
