#ifndef __HW_ENCODER__
#define __HW_ENCODER__

#ifdef __cplusplus
extern "C" {
#endif

typedef enum hw_encoder_mode_e
{
    HW_	ENCODER_MODE1 = 1,//001
	HW_	ENCODER_MODE2,//010
	HW_	ENCODER_MODE3,//011 -  Configure both inputs are active on both rising and falling edges deixar como modo 3
    HW_ENCODER_MODE_MAX,
	
} hw_encoder_mode_t;

//configurações canais
typedef enum hw_encoder_ic_polarity_e// polaridade nao invertida deixar fixo 
{
    HW_ENCODER_POLARITY_RISING_EDGE = 0,//noninverted/rising edge
    HW_ENCODER_POLARITY_FALLING_EDGE = 1,//inverted/falling edge
    HW_ENCODER_POLARITY_MAX,
} hw_encoder_ic_polarity_t;

typedef enum hw_encoder_ic_selection_e//direção do canal - ela é configurada conforme o modo do encoder 01 opção deixar fixo
{
    
    HW_ENCODER_CHANNEL_INPUT_MAPPED_TIX,//IC1 - TI1 E IC2 - TI2
    HW_ENCODER_CHANNEL_INPUT_MAPPED_TIX,////IC1 - TI2 E IC2 - TI1
    HW_ENCODER_IC_SELECTION_MAX,
} hw_encoder_ic_selection_t;

typedef enum hw_encoder_prescaler_division_ratio_e
{
    HW_ENCODER_NO_PRESCALER = 0, 
    HW_ENCODER_DIVISION_2,
	HW_ENCODER_DIVISION_4,
	HW_ENCODER_DIVISION_8,
	HW_ENCODER_PRESCALAR_DIVISION_MAX,

} hw_encoder_prescaler_division_ratio_t;

bool hw_encoder_calc_presc_period(uint32_t frequency, uint16_t *preescalar, uint16_t *period);
bool hw_encoder_base(hw_encoder_timers_t timer, uint32_t frequency);
void hw_encoder_start(hw_encoder_timers_t timer);
void hw_encoder_stop(hw_encoder_timers_t timer);
void hw_encoder_init(hw_encoder_timers_t timer, hw_encoder_mode_t encoder_mode);
void hw_encoder_reset();//CNT -16 bits 

//CONFIGURAÇÕES CANAIS
void hw_encoder_ic_polarity(void);
void hw_encoder_ic_selection(hw_encoder_timers_t timer, hw_encoder_ic_selection_t ic_selection);
void hw_encoder_prescaler_division(hw_encoder_timers_t timer, hw_encoder_prescaler_division_ratio_t prescaler_division_ratio);
void hw_encoder_input_filter(hw_encoder_timers_t timer, uint_32_t input_filter);
uint32_t hw_v_cont(hw_encoder_timers_t timer);

#ifdef __cplusplus
}
#endif

#endif /* __HW_ENCODER__ */