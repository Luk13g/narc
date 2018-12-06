#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "hw_adc.h"
#include "stm32l052xx.h"

ADC_TypeDef *hw_adc_map[HW_ADC_MAX] = 
{
    ADC1,
    NULL,
    NULL,
};

static bool hw_adc_is_valid(hw_adc_t adc)//verifica se o numeros de adc, não ultrapassa o maior
{
    bool ret = false;

    if((adc < HW_ADC_MAX) && (adc < HW_ADC_MAX))
    {
        if(hw_adc_map[adc])
            ret = true;
    }

    return ret;
}

uint16_t hw_calibration_adc(hw_adc_t adc)// verifica se a calibração do adc foi completa ou não
{
    uint32_t cal_val = 0;

    if(hw_adc_is_valid(adc))
        cal_val = hw_adc_map[adc]->ISR & (1 << 11) ? 1 : 0;

    return cal_val;
}

uint16_t hw_adc_read(hw_adc_t adc)//retorna o valor ja convertido em adc
{
	uint32_t v_adc = 0;
	
	v_adc = hw_adc_map[adc]-> DR;
	return v_adc;
}

bool hw_adc_conf(hw_adc_t adc);
{
 if(!hw_adc_is_valid(adc))
		return false;
	
 hw_adc_map[adc]-> CFGR2 &= ~ (0x03 << 30);// ADC clock mode - PCLK (Synchronous clock mode). This configuration must be enabled only if PCLK 
 hw_adc_map[adc]-> CFGR2 |= (0x03 << 30);

 if((hw_adc_map[adc] -> CR & (0x01 << 0)) == true )//condição para ver se o meu adc ligado, desliga antes de calibrar o meu adc
 {
  hw_adc_map[adc]-> CR &= ~ (0x01 << 0); 
 }

  hw_adc_map[adc]-> CR |= (0x01 << 31);// ADC calibration - 1: Write 1 to calibrate the ADC. Read at 1 means that a calibration is in progress. 

  while(hw_calibration_adc(adc) == false )// End Of Calibration flag  -verifica se a calibração foi completa, senão fica preso no laço
  {
  }

  hw_adc_map[adc]-> ISR |= (0x01 << 0);// ADC is ready to start conversion

  hw_adc_map[adc]-> CR |= (0x01 << 0);

  hw_adc_map[adc]-> CFGR1 |= (0x01 << 13);// mode de conversão é continuo - O ADC converte os canais continuamente, sem qualquer intervenção da CPU.

  hw_adc_map[adc]-> SMPR &= ~(0x07 << 0);//Sampling time selection - 000: 1.5 ADC clock cycles 

  hw_adc_map[adc]-> CHSELR |= (0x01 << 8);// ativa o canal utilizado, que no cao é o canal 8
 
  hw_adc_map[adc]-> CFGR1 &= ~(0x03 << 10);//conversão por software
    
  hw_adc_map[adc]-> CR |= (0x01 << 2);// comando que inicia a conversão adc
 
  return true;
}