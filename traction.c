/*
 * brief   : This module has traction functions
 * date    : 21/April/2023
 * authors :
 * 			Luis Roberto Vega Campos &
 * 			Luis Ruben Padilla Ortiz &
 * 			Arlette Gabriela Uribe Ventura
 */

#include "traction.h"



volatile static traction_t          g_traction      = {0, forward_t};
volatile static function_traction_t g_callback      = 0;
volatile static uint8_t             g_speed_divider = DIVIDER_NORMAL;

void init_pwm()
{
	static ftm_config_t config;
	FTM_GetDefaultConfig(&config);
	config.prescale = kFTM_Prescale_Divide_4;
	config.bdmMode = kFTM_BdmMode_3;
	FTM_Init(FLEX_TIMER_TO_USE, &config);
	FTM_EnableInterrupts(FLEX_TIMER_TO_USE, kFTM_TimeOverflowInterruptEnable);
    FTM_StartTimer(FLEX_TIMER_TO_USE, kFTM_SystemClock);
}

void traction_init()
{
	uint32_t clock_frec = CLOCK_GetFreq(kCLOCK_BusClk);

	CLOCK_EnableClock(PORT_CLOCK);
	PORT_SetPinMux(PORT_PWM_CH1, PIN_PWM_CH1, PORT_ALTERNATIVE);

	uint32_t duty_cycle = timeus_to_duty(1500);

	ftm_chnl_pwm_signal_param_t pwm_config =
	{
			FLEX_TIMER_CHANNEL,
			kFTM_HighTrue,
			duty_cycle,
			0u,
			false,
			false
	};

	FTM_StopTimer(FLEX_TIMER_TO_USE);
	FTM_SetupPwm(FLEX_TIMER_TO_USE, &pwm_config, 1u, kFTM_EdgeAlignedPwm, PWM_FRECUENCY, clock_frec);
	FLEX_TIMER_TO_USE->CONTROLS[FLEX_TIMER_CHANNEL].CnV = USEC_TO_COUNT(NEUTRAL_PWM_US, clock_frec/4);
	FTM_StartTimer(FLEX_TIMER_TO_USE, kFTM_SystemClock);
}

void set_traction(traction_t traction)
{
	FTM_StopTimer(FLEX_TIMER_TO_USE);
	if(MAX_SPEED < traction.speed)
	{
		traction.speed = MAX_SPEED;
	}
	else if(0 > traction.speed)
	{
		traction.speed = 0;
	}
	traction.speed /= g_speed_divider;
	uint32_t clock_frec = CLOCK_GetFreq(kCLOCK_BusClk);
	g_traction.speed = NEUTRAL_PWM_US;
	g_traction.direction = traction.direction;
	g_traction.speed += ((traction.direction == forward_t)? (traction.speed):(-traction.speed));
	FLEX_TIMER_TO_USE->CONTROLS[FLEX_TIMER_CHANNEL].CnV = USEC_TO_COUNT(g_traction.speed, clock_frec/4)+OFFSET;
	FLEX_TIMER_TO_USE->CNTIN = 0u;
	FTM_SetSoftwareTrigger(FLEX_TIMER_TO_USE, true);
	FTM_StartTimer(FLEX_TIMER_TO_USE, kFTM_SystemClock);
}

traction_t get_traction()
{
	traction_t ret_val =
	{
			g_traction.direction-NEUTRAL_PWM_US,
			g_traction.speed
	};
	ret_val.speed = (0 < ret_val.speed)? ret_val.speed : ret_val.speed*-1;
	return ret_val;
}

void set_sport_traction()
{
	g_speed_divider = DIVIDER_SPORT;
}

void set_normal_traction()
{
	g_speed_divider = DIVIDER_NORMAL;
}

void set_slow_traction()
{
	g_speed_divider = DIVIDER_SLOW;
}

void set_callback(function_traction_t callback)
{
	g_callback = callback;
}

void FTM0_IRQHandler()
{
	if(0 != g_callback)
	{
		g_callback();
	}
	FTM_ClearStatusFlags(FLEX_TIMER_TO_USE, kFTM_TimeOverflowFlag);
}



