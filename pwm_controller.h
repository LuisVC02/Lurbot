/*
 * date   : 12/June/2023
 * author : Luis Roberto Vega Campos &
 * 			Luis Rubén Padilla Ortiz &
 * 			Arlette Gabriela Uribe Ventura
 * brief  : This module have functions for generate PWM with FlexTimer
 */

#ifndef PWM_CONTROLLER_H_
#define PWM_CONTROLLER_H_

#include "fsl_ftm.h"

#define N_TIMERS   4
#define N_CHANNELS 8
#define timeus_to_duty(time) (100*time)/(1000000/PWM_FRECUENCY)

typedef enum
{
	FlexTimer0,
	FlexTimer1,
	FlexTimer2,
	FlexTimer3
}flex_timer_t;

void pwm_init(flex_timer_t timer, ftm_clock_prescale_t prescaler);

void pwm_config(flex_timer_t timer, ftm_chnl_t channel, ftm_pwm_mode_t pwm_mode, ftm_pwm_level_select_t level, uint32_t frecuency, uint32_t time_us);

void pwm_set_time(flex_timer_t timer, ftm_chnl_t channel, uint32_t time_us);

#endif /* PWM_CONTROLLER_H_ */
