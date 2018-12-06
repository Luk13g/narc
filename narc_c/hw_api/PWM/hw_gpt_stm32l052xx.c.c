#include <stdint.h>
#include <stdbool.h>
#include "hw_api.h"
#include "hw_gpio.h"
#include "hw_gpt.h"
#include"stm32l0xx_hal_rcc.h"
#include "stm32l052xx.h"
#include <stdio.h>

TIM_TypeDef *hw_gpt_map[HW_GPT_TIM_MAX] = {
NULL,
TIM2,
NULL,
NULL,
NULL,
TIM6,
NULL,
NULL,
NULL,
NULL,
NULL,
NULL,
NULL,
NULL,
NULL,
NULL,
NULL,
NULL,
NULL,
NULL,
TIM21,
TIM22, };

static bool hw_gpt_is_valid(hw_gpt_timers_t timer) {
	bool ret = false;

	if (timer < HW_GPT_TIM_MAX) {
		if (hw_gpt_map[timer])
			ret = true;
	}

	return ret;
}

uint8_t hw_pwm_calc_presc_period(uint32_t frequency, uint16_t *preescalar,
		uint16_t *period) {
	uint32_t timer_frequency = HAL_RCC_GetPCLK2Freq(); //pega o valor referente ao clock
	uint32_t base_frequency = timer_frequency;
	uint16_t _pre = 0;
	uint32_t _per = 0;

	if (frequency < 1 || frequency > timer_frequency) //determina um limite para frequencia sendo de 1 até menor que o valor do clock
			{
		return 0;
	}
	do {
		_per = base_frequency / frequency;
		if (_per < 0xFFFF) //65536 - limite de 16 bits
				{
			_pre += 1;
			base_frequency = timer_frequency / (_pre + 1);
		}
	} while (_per < 0xFFFF);
	*preescalar = _pre;
	*period = _per;
	return 1;
}

void hw_gpt_disable(hw_gpt_timers_t timer, hw_gpt_channels_t channel) {

	hw_gpt_map[timer]->CCER &= ~(1 << (channel * 4));

}

void hw_gpt_enable(hw_gpt_timers_t timer, hw_gpt_channels_t channel) {

	hw_gpt_map[timer]->CCER |= (1 << (channel * 4));

}

void hw_gpt_configure_channel(hw_gpt_timers_t timer, hw_gpt_channels_t channel) {
	if (channel == HW_GPT_CHANNEL1) {

		hw_gpt_map[timer]->CCMR1 |= (1 << 3);
		hw_gpt_map[timer]->CCMR1 &= ~(0x15 << 4); //~(1111) = 0000
		hw_gpt_map[timer]->CCMR1 |= (0x06 << 4); // 0000 | 0110 = 0110
		hw_gpt_map[timer]->CCR1 = 0x00;  //zera o ciclo, 16 bits

	} else if (channel == HW_GPT_CHANNEL2) {
		hw_gpt_map[timer]->CCMR1 |= (1 << 11);
		hw_gpt_map[timer]->CCMR1 &= ~(0x15 << 12);
		hw_gpt_map[timer]->CCMR1 |= (0x06 << 12);
		hw_gpt_map[timer]->CCR2 = 0x00;

	} else if (channel == HW_GPT_CHANNEL3) {
		hw_gpt_map[timer]->CCMR2 |= (1 << 3);
		hw_gpt_map[timer]->CCMR1 &= ~(0x15 << 4);
		hw_gpt_map[timer]->CCMR2 |= (0x06 << 4);
		hw_gpt_map[timer]->CCR3 = 0x00;

	} else if (channel == HW_GPT_CHANNEL4) {
		hw_gpt_map[timer]->CCMR2 |= (1 << 11);
		hw_gpt_map[timer]->CCMR1 &= ~(0x15 << 12);
		hw_gpt_map[timer]->CCMR2 |= (0x06 << 12);
		hw_gpt_map[timer]->CCR4 = 0x00;

	}

}

bool hw_gpt_configure(hw_gpt_timers_t timer, uint32_t frequency) {

	if (!hw_gpt_is_valid(timer))
		return false;

	// hw_gpio_pin_set_mode(port,pin,HW_GPIO_PIN_ALTERNATE_FUNCTION); deixar para pessoa setar no main
	hw_gpt_map[timer]->CR1 &= ~(1 << 0);
	hw_gpt_map[timer]->CR1 |= (1 << 0); //Ativar o periférico do TIM no bit 0 coloca 1

	uint16_t preescalar, period;

	if (hw_pwm_calc_presc_period(frequency, &preescalar, &period) == 0)
		return false;

	hw_gpt_map[timer]->PSC = preescalar;
	hw_gpt_map[timer]->EGR = (1 << 0); /* Gerar um evento de atualização para recarregar o Prescaler
	 e o valor do contador de repetições imediatamente, no bit 0 coloca 1*/

	hw_gpt_map[timer]->ARR = period;

	return true;
}
bool hw_gpt_cal_duty_cycle(hw_gpt_timers_t timer, hw_gpt_channels_t channel,
		uint8_t duty_cycle_per) //determinar o duty cyclo referente ao canal, varia de 0 a 100% do valor do periodo.
{
	uint16_t ccr;
	if (duty_cycle_per < 0 || duty_cycle_per > 100)
		return false;
	ccr = (duty_cycle_per * (hw_gpt_map[timer]->ARR + 1) - 1) / 100; //meu arr vai ser o meu periodo

	if (channel == HW_GPT_CHANNEL1) {
		hw_gpt_map[timer]->CCR1 = ccr;

	} else if (channel == HW_GPT_CHANNEL2) {
		hw_gpt_map[timer]->CCR2 = ccr;

	} else if (channel == HW_GPT_CHANNEL3) {
		hw_gpt_map[timer]->CCR3 = ccr;

	} else if (channel == HW_GPT_CHANNEL4) {
		hw_gpt_map[timer]->CCR4 = ccr;

	}
    return true;
}

