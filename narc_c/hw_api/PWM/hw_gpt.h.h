#ifndef __HW_GPT__
#define __HW_GPT__

#ifdef __cplusplus
extern "C" {
#endif


typedef enum hw_gpt_timers_e
{
    HW_GPT_TIM1 = 0,
    HW_GPT_TIM2,
    HW_GPT_TIM3,
    HW_GPT_TIM4,
    HW_GPT_TIM5,
    HW_GPT_TIM6,
    HW_GPT_TIM7,
    HW_GPT_TIM8,
    HW_GPT_TIM9,
    HW_GPT_TIM10,
    HW_GPT_TIM11,
    HW_GPT_TIM12,
    HW_GPT_TIM13,
    HW_GPT_TIM14,
    HW_GPT_TIM15,
    HW_GPT_TIM16,
    HW_GPT_TIM17,
    HW_GPT_TIM18,
    HW_GPT_TIM19,
    HW_GPT_TIM20,
    HW_GPT_TIM21,
    HW_GPT_TIM22,
    HW_GPT_TIM_MAX,
} hw_gpt_timers_t;

typedef enum hw_gpt_channels_e
{
    HW_GPT_CHANNEL1 = 0,
    HW_GPT_CHANNEL2,
    HW_GPT_CHANNEL3,
    HW_GPT_CHANNEL4,
    HW_GPT_CHANNEL5,
    HW_GPT_CHANNEL_MAX,
} hw_gpt_channels_t;

void hw_gpt_disable (hw_gpt_timers_t timer, hw_gpt_channels_t channel);
void hw_gpt_enable (hw_gpt_timers_t timer, hw_gpt_channels_t channel);
void hw_gpt_configure_channel (hw_gpt_timers_t timer, hw_gpt_channels_t channel);
bool hw_gpt_configure(hw_gpt_timers_t timer,uint32_t frequency);
bool hw_gpt_set_duty_cycle(uint8_t perc);
uint8_t hw_pwm_calc_presc_period(uint32_t frequency, uint16_t *preescalar, uint16_t *period);


#ifdef __cplusplus
}
#endif

#endif /* __HW_GPT__ */