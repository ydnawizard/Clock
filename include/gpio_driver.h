#include "stm32f4xx.h"
#include <stddef.h>
#ifndef GPIO_DRIVER_H_
#define GPIO_DRIVER_H_
#ifndef __COMPILER_BARRIER
  #define __COMPILER_BARRIER() __asm__ volatile("" ::: "memory")
#endif

#include "stm32f4xx.h"
#include <stdint.h>

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
				   //
static void gpio_clock_en(GPIO_TypeDef * GPIOx);

static void gpio_clock_ds(GPIO_TypeDef * GPIOx);

void gpio_pin_cfg(GPIO_TypeDef * GPIOx, uint8_t pin, uint8_t speed, uint8_t mode);

void gpio_pin_alt_cfg(GPIO_TypeDef * GPIOx, uint8_t pin, uint8_t function);

static GPIO_TypeDef * gpio_get_port(char port);

void gpio_init(char port, uint8_t pin_number, uint8_t speed, uint8_t mode);

void gpio_ds(char port);

void gpio_init_targets(void);

#endif
