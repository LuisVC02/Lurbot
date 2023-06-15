/*
 * date   : 12/June/2023
 * author : Luis Roberto Vega Campos &
 * 			Luis Rubén Padilla Ortiz &
 * 			Arlette Gabriela Uribe Ventura
 * brief  :
 */

#include "direction.h"

volatile static int8_t g_angle        = 0;

void init_direction()
{
	CLOCK_EnableClock(PORT_CLOCK_DIRECTION);
	PORT_SetPinMux(PWM_PORT_DIRECTION, PWM_PIN_DIRECTION, PWM_ALTERNATIVE_DIRECTION);
	pwm_init(PWM_TIMER_DIRECTION, kFTM_Prescale_Divide_4);
	pwm_config(PWM_TIMER_DIRECTION, PWM_CHANNEL_DIRECTION, kFTM_EdgeAlignedPwm, kFTM_HighTrue, PWM_FRECUENCY_DIRECTION, NEUTRAL_PWM_DIRECTION);
}

bool set_angle(int16_t angle)
{
	if(MAX_ANGLE_DIRECTION >= angle && MIN_ANGLE_DIRECTION <= angle)
	{
		g_angle = angle * -1;
		angle *= ANGLE_TO_PWM_GAIN_DIRECTION;
		angle += NEUTRAL_PWM_DIRECTION;
		angle += PWM_OFFSET;
		pwm_set_time(PWM_TIMER_DIRECTION, PWM_CHANNEL_DIRECTION, angle);
		return true;
	}
	return false;
}

bool set_pwm_direction_time(uint16_t time)
{
	if(MAX_PWM_DIRECTION >= time && MIN_PWM_DIRECTION <= time)
	{
		time += PWM_OFFSET;
		pwm_set_time(PWM_TIMER_DIRECTION, PWM_CHANNEL_DIRECTION, time);
		return true;
	}
	return false;
}

int8_t get_angle()
{
	return g_angle;
}
