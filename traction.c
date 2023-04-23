/*
 * brief   : This module has traction functions
 * date    : 21/April/2023
 * authors :
 * 			Luis Roberto Vega Campos &
 * 			Luis Ruben Padilla Ortiz &
 * 			Arlette Gabriela Uribe Ventura
 */

#include "traction.h"

#define timeus_to_duty(time) (100*time)/(1000000/PWM_FRECUENCY)

volatile static traction_t g_traction = {0, forward_t};

void traction_init()
{
	static ftm_config_t config;
	uint32_t clock_frec = CLOCK_GetFreq(kCLOCK_BusClk);

	CLOCK_EnableClock(PORT_CLOCK);
	PORT_SetPinMux(PORT_PWM_CH1, PIN_PWM_CH1, PORT_ALTERNATIVE);


	FTM_GetDefaultConfig(&config);
	config.prescale = kFTM_Prescale_Divide_2;
	config.bdmMode = kFTM_BdmMode_3;
	FTM_Init(FLEX_TIMER_TO_USE, &config);

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

	FTM_SetupPwm(FLEX_TIMER_TO_USE, &pwm_config, 1u, kFTM_CenterAlignedPwm, PWM_FRECUENCY, clock_frec);
	FLEX_TIMER_TO_USE->CONTROLS[FLEX_TIMER_CHANNEL].CnV = USEC_TO_COUNT(NEUTRAL_PWM_US, clock_frec/2);
    FTM_StartTimer(FLEX_TIMER_TO_USE, kFTM_SystemClock);
}

bool set_traction(traction_t traction)
{
	if(MAX_SPEED >= traction.speed && 0 <= traction.speed)
	{
		uint32_t clock_frec = CLOCK_GetFreq(kCLOCK_BusClk);
		g_traction.speed = NEUTRAL_PWM_US;
		g_traction.direction = traction.direction;
		g_traction.speed += ((traction.direction == forward_t)? (traction.speed):(-traction.speed));
		FLEX_TIMER_TO_USE->CONTROLS[FLEX_TIMER_CHANNEL].CnV = USEC_TO_COUNT(g_traction.speed, clock_frec/2);
		FTM_SetSoftwareTrigger(FLEX_TIMER_TO_USE, true);
		return true;
	}
	else
	{
		return false;
	}
}

traction_t get_traction()
{
	return g_traction;
}


