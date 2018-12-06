/*
 * led.c
 *
 *  Created on: 4 de nov de 2018
 *      Author: lukas
 */
#include <stdint.h>
#include <stdbool.h>
#include "hw_api.h"
#include "hw_gpio.h"
#include "stm32l052xx.h"


void gpio_init(void)
{
	//hw_gpio_pin_set(HW_GPIO_PORTA, HW_GPIO_PIN5);
	hw_gpio_pin_set_mode(HW_GPIO_PORTA, HW_GPIO_PIN5, HW_GPIO_PIN_OUTPUT);
	hw_gpio_pin_set_topology(HW_GPIO_PORTA, HW_GPIO_PIN5, HW_GPIO_PIN_TOPOLOGY_PUSH_PULL);
	hw_gpio_pin_set_pupd(HW_GPIO_PORTA, HW_GPIO_PIN5,HW_GPIO_PIN_PUPD_NOPULL);
	hw_gpio_pin_set_speed(HW_GPIO_PORTA, HW_GPIO_PIN5,HW_GPIO_PIN_SPEED_LOW);

	hw_gpio_pin_set_mode(HW_GPIO_PORTA, HW_GPIO_PIN4, HW_GPIO_PIN_INPUT);
	hw_gpio_pin_set_pupd(HW_GPIO_PORTA, HW_GPIO_PIN4,HW_GPIO_PIN_PUPD_UP);


}
void loop(void)
{
     if(hw_gpio_pin_read(HW_GPIO_PORTA,HW_GPIO_PIN4)==HW_GPIO_PIN_SET)
     {
    	 hw_gpio_pin_set(HW_GPIO_PORTA, HW_GPIO_PIN5);
     }
     else
     {
    	 hw_gpio_pin_reset(HW_GPIO_PORTA, HW_GPIO_PIN5);
     }
}
