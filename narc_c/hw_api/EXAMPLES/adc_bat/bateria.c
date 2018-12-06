/*
 * bateria.c
 *
 *  Created on: 5 de dez de 2018
 *      Author: lukas
 */

#include <stdint.h>
#include <stdbool.h>
#include "hw_adc.h"
#include "stm32l052xx.h"

uint32_t adc_data_bat = 0;
uint32_t tens_bat = 0;

void adc_init(void)
{
	hw_adc_conf(HW_ADC1);
}

uint32_t calc_bateria()
{
	adc_data_bat = hw_adc_read(HW_ADC1);
    tens_bat = ((3.3 * adc_data_bat)/4096)*1000;
    return tens_bat;
}

void loop(void)
{
	calc_bateria();

}


