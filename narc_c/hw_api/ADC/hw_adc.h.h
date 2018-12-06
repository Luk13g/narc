#ifndef __HW_ADC__
#define __HW_ADC__

#ifdef __cplusplus
extern "C" {
#endif

typedef enum hw_adc_e
{
    HW_ADC1 = 0,
    HW_ADC2,
    HW_ADC3,
    HW_ADC_MAX
} hw_adc_t;

uint16_t hw_adc_read(hw_adc_t adc);
bool hw_adc_conf(hw_adc_t adc);

#ifdef __cplusplus
}
#endif

#endif /* __HW_ENCODER__ */