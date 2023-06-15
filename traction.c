/*
 * date   : 12/June/2023
 * author : Luis Roberto Vega Campos &
 * 			Luis Rubén Padilla Ortiz &
 * 			Arlette Gabriela Uribe Ventura
 * brief  : This module has functions to control driver of DC motor
 */

#include "traction.h"

volatile static int8_t g_speed        = 0;
volatile static int8_t g_speed_sensor = 0;

void init_traction()
{
	CLOCK_EnableClock(PORT_CLOCK_TRACTION);
	PORT_SetPinMux(PWM_PORT_TRACTION, PWM_PIN_TRACTION, PWM_ALTERNATIVE_TRACTION);
	pwm_init(PWM_TIMER_TRACTION, kFTM_Prescale_Divide_4);
	pwm_config(PWM_TIMER_TRACTION, PWM_CHANNEL_TRACTION, kFTM_EdgeAlignedPwm, kFTM_HighTrue, PWM_FRECUENCY_TRACTION, NEUTRAL_PWM_TRACTION);
}

bool set_speed(int16_t speed)
{
	if(MAX_SPEED_TRACTION >= speed && MIN_SPEED_TRACTION <= speed)
	{
		g_speed = (int8_t)speed;
		speed *= SPEED_TO_PWM_GAIN_TRACTION;
		speed += NEUTRAL_PWM_TRACTION;
		pwm_set_time(PWM_TIMER_TRACTION, PWM_CHANNEL_TRACTION, speed);
		return true;
	}
	return false;
}

bool set_pwm_traction_time(uint16_t time)
{
	if(MAX_PWM_TRACTION >= time && MIN_PWM_TRACTION <= time)
	{
		pwm_set_time(PWM_TIMER_TRACTION, PWM_CHANNEL_TRACTION, time);
		return true;
	}
	return false;
}

int8_t get_speed_sensor()
{
	return 0;
}

int8_t get_speed()
{
	return g_speed;
}
