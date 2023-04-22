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

volatile static uint16_t              g_speed = 0;
volatile static direction_traction_t  g_direction = forward_t;

void traction_init()
{
	static ftm_config_t config;
	uint32_t clock_frec = CLOCK_GetFreq(kCLOCK_BusClk);

	CLOCK_EnableClock(PORT_CLOCK);
	PORT_SetPinMux(PORT_PWM_CH1, PIN_PWM_CH1, PORT_ALTERNATIVE);


	FTM_GetDefaultConfig(&config);
	config.prescale = FTM_CalculateCounterClkDiv(FLEX_TIMER_TO_USE, PWM_FRECUENCY, clock_frec);
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
    FTM_StartTimer(FLEX_TIMER_TO_USE, kFTM_SystemClock);
}

void set_speed(traction_t traction)
{
	if(traction.speed <= MAX_SPEED)
	{
		g_speed = NEUTRAL_PWM_US;
		g_direction = traction.direction;
		g_speed += (traction.direction == forward_t)? (traction.speed):(-traction.speed);
		uint32_t duty_cycle = timeus_to_duty(g_speed);
		FTM_UpdatePwmDutycycle(FLEX_TIMER_TO_USE, FLEX_TIMER_CHANNEL, kFTM_CenterAlignedPwm, duty_cycle);
		FTM_SetSoftwareTrigger(FLEX_TIMER_TO_USE, true);
	}
}

void increase_speed(traction_t traction)
{

}

void decrease_speed(traction_t traction)
{

}

void stop_motor()
{

}

