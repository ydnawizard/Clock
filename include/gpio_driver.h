
#ifndef GPIO_DRIVER_H_
#define GPIO_DRIVER_H_
#ifndef __COMPILER_BARRIER
  #define __COMPILER_BARRIER() __asm__ volatile("" ::: "memory")
#endif

#include "stm32f4xx.h"
#include <stdint.h>

static void gpio_clock_enable(GPIO_TypeDef * GPIOx);

static void gpio_clock_disable(GPIO_TypeDef * GPIOx);

void gpio_pin_configure(GPIO_TypeDef * GPIOx, uint8_t pin, uint8_t mode);

static GPIO_TypeDef * get_gpio_port(char port);

void gpio_init(char port, uint8_t pin_number, uint8_t mode);

void gpio_disable(char port);

void gpio_init_a567(void);

#endif
