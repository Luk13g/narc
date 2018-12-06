#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "hw_api.h"
#include "hw_gpio.h"
#include "hw_encoder.h"
#include "stm32l052xx.h"

TIM_TypeDef *hw_encoder_map[HW_ENCODER_TIM_MAX] = 
{
    NULL,
    TIM2,
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
    NULL,
    NULL,
    NULL,
    NULL,
    TIM21,
    TIM22,
};

uint8_t hw_encoder_ic_selection_map[HW_ENCODER_IC_SELECTION_MAX] = 
{
	0x00, 
    0x01,
    0x02,
    0x03,
};
uint8_t hw_encoder_prescaler_division_map[HW_ENCODER_PRESCALAR_DIVISION_MAX] = 
{
	0x00, 
    0x01,
    0x02,
    0x03,
};

static bool hw_encoder_is_valid(hw_encoder_timers_t timer)
{
    bool ret = false;

    if(timer < HW_ENCODER_TIM_MAX)
    {
        if(hw_encoder_map[timer])
            ret = true;
    }

	

    return ret;
}

bool hw_encoder_calc_presc_period(uint32_t frequency, uint16_t *preescalar, uint16_t *period)
{
    uint32_t timer_frequency = HAL_RCC_GetPCLK2Freq(); //pega o valor referente ao clock
    uint32_t base_frequency = timer_frequency;
    uint16_t _pre = 0;
    uint32_t _per = 0;
    
    if(frequency < 1 || frequency > timer_frequency)//determina um limite para frequencia sendo de 1 até menor que o valor do clock
    {
        return 0;
    }
    do
    {
        _per = base_frequency / frequency;
        if(_per < 0xFFFF)//65536 - limite de 16 bits
        {
            _pre += 1;
            base_frequency = timer_frequency / (_pre + 1);
        }
    }
    while(_per < 0xFFFF);
    *preescalar = _pre;
    *period = _per;
    return 1;
}

bool hw_encoder_base(hw_encoder_timers_t timer, uint32_t channel, uint32_t frequency)
{
    
    if(!hw_encoder_is_valid(timer))
		return false;
	
    hw_encoder_map[timer]->CR1 &= ~ (1 << 0);
    hw_encoder_map[timer]->CR1 |= (1 << 0);
   
    uint16_t preescalar, period;
    
    if(hw_pwm_calc_presc_period(frequency, &preescalar, &period) == 0)
        return  0;

    hw_encoder_map[timer]->ARR = period;
    hw_encoder_map[timer]->PSC = preescalar;
	hw_encoder_map[timer]->EGR = (1 << 0); 
     
}


void hw_encoder_stop (hw_gpt_timers_t timer)//habiliatr ou disabilitar os 2 ao mesmo tempo
{                    
  //channel 1
  hw_encoder_map[timer]->CCER &= ~(1 << 0);             
  //channel 2
  hw_encoder_map[timer]->CCER &= ~(1 << 4);  
}

void hw_encoder_start (hw_gpt_timers_t timer)
{
   //channel 1
   hw_encoder_map[timer]->CCER |= (1 << 0); 
   //channel 2
   hw_encoder_map[timer]->CCER |= (1 << 4);   
}

void hw_encoder_init()
{
	hw_encoder_map[timer]-­>SMCR &= ~(0x07 << 0);
	hw_encoder_map[timer]-­>SMCR |= (0x03 << 0);

}

void hw_encoder_reset()
{
hw_encoder_map[timer]->CNT = 0x00; 
}

void hw_encoder_ic_polarity()//deixar fixa sem parametro, e com polaridade nao invertida
{
  /*bit 1 e 3
  00 rising
  01 falling
  bit 5 e 7 canal 2
  CC1NP-0/CC1P -1 
  */
  //channel 1
  hw_encoder_map[timer] -> CCER & = ~ (1<<1);  //TIM_CCER_CC1P - bit 1  TIM_CCER_CC2P);
  hw_encoder_map[timer] -> CCER &= ~ (1<<3);      //(TIM_CCER_CC1NP | TIM_CCER_CC2NP);
  //channel 2
  hw_encoder_map[timer] -> CCER & = ~ (1<<5);     //(TIM_CCER_CC1P | TIM_CCER_CC2P);
  hw_encoder_map[timer] -> CCER &= ~ (1<<7); 
}

void hw_encoder_ic_selection(hw_encoder_timers_t timer, uint32_t channel1, uint32_t channel2, hw_encoder_ic_selection_t ic_selection)//fixo
{
  //channel 1
  hw_encoder_map[timer] ->CCMR1  &= ~(0x03<< 0);
  hw_encoder_map[timer] ->CCMR1  |= (hw_encoder_ic_selection_map[ic_selection]<< 0);
  //channel 2
  hw_encoder_map[timer] ->CCMR1 &= ~(0x03<< 8);
  hw_encoder_map[timer] ->CCMR1  |= (hw_encoder_ic_selection_map[ic_selection]<< 0);
}

void hw_encoder_prescaler_division(hw_encoder_timers_t timer,hw_encoder_prescaler_division_ratio_t prescaler_division_ratio)//pode tirar se quiser
{
  //channel 1
  hw_encoder_map[timer] ->CCMR1 &= ~(0x03 << 2);    
  hw_encoder_map[timer] ->CCMR1 |= (hw_encoder_prescaler_division_map[prescaler_division_ratio]<< 2);
  //channel 2
  hw_encoder_map[timer] ->CCMR1 &= ~(0x03 << 10);
  hw_encoder_map[timer] ->CCMR1 |= (hw_encoder_prescaler_division_map[prescaler_division_ratio]<< 10);
}

void hw_encoder_input_filter(hw_encoder_timers_t timer, uint32_t input_filter)//pode tirar se quiser
{
  //channel 1
  hw_encoder_map[timer] ->CCMR1 &= ~(0x07 << 4);
  hw_encoder_map[timer] ->CCMR1 |= (input_filter << 4);
  //channel 2
  hw_encoder_map[timer] ->CCMR1 &= ~(0x07 << 12);
  hw_encoder_map[timer] ->CCMR1 |= (input_filter << 12);
}

uint32_t hw_v_cont(hw_encoder_timers_t timer)
{
 uint32_t cnt = 0;
 cnt = hw_encoder_map[timer]->CNT;
 
 return cnt;
}
//colocar uma função que volta o valor do  do contador 




	