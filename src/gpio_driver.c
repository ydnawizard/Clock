#include "stm32f4xx.h"
#include <stddef.h>

/******************************************************************************
 * MACROS & CONSTANTS
 ******************************************************************************/
// RCC AHB1 Enable Bit Masks (STM32F401 supports GPIO Ports A, B, C, D, E, H)
#define GPIOAEN         (1U << 0)
#define GPIOBEN         (1U << 1)
#define GPIOCEN         (1U << 2)
#define GPIODEN         (1U << 3)
#define GPIOEEN         (1U << 4)
#define GPIOHEN         (1U << 7)

// Pin Modes for STM32F4 MODER
#define GPIO_MODE_INPUT     0x00U  // 00: Input
#define GPIO_MODE_OUTPUT    0x01U  // 01: General purpose output
#define GPIO_MODE_ALT       0x02U  // 10: Alternate function
#define GPIO_MODE_ANALOG    0x03U  // 11: Analog mode

/******************************************************************************
 * PORT CLOCK CONTROL
 ******************************************************************************/
static void GPIO_Clock_Enable(GPIO_TypeDef *GPIOx)
{
    if (GPIOx == GPIOA)      RCC->AHB1ENR |= GPIOAEN;
    else if (GPIOx == GPIOB) RCC->AHB1ENR |= GPIOBEN;
    else if (GPIOx == GPIOC) RCC->AHB1ENR |= GPIOCEN;
    else if (GPIOx == GPIOD) RCC->AHB1ENR |= GPIODEN;
    else if (GPIOx == GPIOE) RCC->AHB1ENR |= GPIOEEN;
    else if (GPIOx == GPIOH) RCC->AHB1ENR |= GPIOHEN;
}

static void GPIO_Clock_Disable(GPIO_TypeDef *GPIOx)
{
    if (GPIOx == GPIOA)      RCC->AHB1ENR &= ~GPIOAEN;
    else if (GPIOx == GPIOB) RCC->AHB1ENR &= ~GPIOBEN;
    else if (GPIOx == GPIOC) RCC->AHB1ENR &= ~GPIOCEN;
    else if (GPIOx == GPIOD) RCC->AHB1ENR &= ~GPIODEN;
    else if (GPIOx == GPIOE) RCC->AHB1ENR &= ~GPIOEEN;
    else if (GPIOx == GPIOH) RCC->AHB1ENR &= ~GPIOHEN;
}

/******************************************************************************
 * CORE CONFIGURE LOGIC
 ******************************************************************************/
void GPIO_Pin_Configure(GPIO_TypeDef *GPIOx, uint8_t pin, uint8_t mode)
{
    if (pin > 15) return;

    // 1. Ensure clock for the peripheral is enabled
    GPIO_Clock_Enable(GPIOx);

    // 2. Configure Pin Mode in MODER (2 bits per pin)
    uint8_t shift = pin * 2;
    GPIOx->MODER &= ~(0x3U << shift);          // Clear 2-bit mode field
    GPIOx->MODER |= ((uint32_t)mode << shift);  // Set new mode value
}

/******************************************************************************
 * MAIN DRIVER API (COMPATIBLE WRAPPERS)
 ******************************************************************************/
static GPIO_TypeDef* Get_GPIO_Port(char port)
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
void GPIO_Init(char port, uint8_t pin_number, uint8_t mode)
{
    GPIO_TypeDef *GPIOx = Get_GPIO_Port(port);
    if (GPIOx != NULL) {
        GPIO_Pin_Configure(GPIOx, pin_number, mode);
    }
}

// Disables peripheral clock for a port
void GPIO_Disable(char port)
{
    GPIO_TypeDef *GPIOx = Get_GPIO_Port(port);
    if (GPIOx != NULL) {
        GPIO_Clock_Disable(GPIOx);
    }
}

/******************************************************************************
 * HELPER FUNCTIONS
 ******************************************************************************/
void GPIO_INIT_A567(void)
{
    GPIO_Pin_Configure(GPIOA, 5, GPIO_MODE_OUTPUT);
    GPIO_Pin_Configure(GPIOA, 6, GPIO_MODE_OUTPUT);
    GPIO_Pin_Configure(GPIOA, 7, GPIO_MODE_OUTPUT);
}
