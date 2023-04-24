/*
 * brief   : This module has direction functions
 * date    : 21/April/2023
 * authors :
 * 			Luis Roberto Vega Campos &
 * 			Luis Ruben Padilla Ortiz &
 * 			Arlette Gabriela Uribe Ventura
 */

#include "direction.h"

volatile static int16_t g_angle = 0;
volatile static function_direction_t g_callback = 0;



void direction_init()
{
	uint32_t clock_frec = CLOCK_GetFreq(kCLOCK_BusClk);

	CLOCK_EnableClock(PORT_CLOCK_DIRECTION);
	PORT_SetPinMux(PORT_PWM_CH3, PIN_PWM_CH3, PORT_ALTERNATIVE_DIRECTION);

	uint32_t duty_cycle = timeus_to_duty(NEUTRAL_PWM_DIRECTION);

	ftm_chnl_pwm_signal_param_t pwm_config =
	{
			FLEX_TIMER_CHANNEL_DIRECTION,
			kFTM_HighTrue,
			duty_cycle,
			0u,
			false,
			false
	};
	FTM_StopTimer(FLEX_TIMER_TO_USE);
	FTM_SetupPwm(FLEX_TIMER_TO_USE_DIRECTION, &pwm_config, 1u, kFTM_EdgeAlignedPwm, PWM_FRECUENCY_DIRECTION, clock_frec);
	FLEX_TIMER_TO_USE_DIRECTION->CONTROLS[FLEX_TIMER_CHANNEL_DIRECTION].CnV = USEC_TO_COUNT(NEUTRAL_PWM_DIRECTION, clock_frec/4);
	FTM_StartTimer(FLEX_TIMER_TO_USE, kFTM_SystemClock);
}

// Values of -90 to 90
void set_direction(int16_t angle)
{
	FTM_StopTimer(FLEX_TIMER_TO_USE);
	if(MAX_ANGLE < angle)
	{
		angle = MAX_ANGLE;
	}
	else if(MIN_ANGLE > angle)
	{
		angle = MIN_ANGLE;
	}
	uint32_t clock_frec = CLOCK_GetFreq(kCLOCK_BusClk);
	g_angle = NEUTRAL_PWM_DIRECTION;
	angle *= 5;
	g_angle += angle;
	FLEX_TIMER_TO_USE_DIRECTION->CONTROLS[FLEX_TIMER_CHANNEL_DIRECTION].CnV = USEC_TO_COUNT(g_angle, clock_frec/4)+OFFSET_DIRECTION;
	FLEX_TIMER_TO_USE_DIRECTION->CNTIN = 0u;
	FTM_SetSoftwareTrigger(FLEX_TIMER_TO_USE_DIRECTION, true);
	FTM_StartTimer(FLEX_TIMER_TO_USE, kFTM_SystemClock);
}

int16_t get_direction()
{
	return g_angle-NEUTRAL_PWM_DIRECTION;
}

