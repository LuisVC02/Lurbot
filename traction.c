/*
 * date   : 12/June/2023
 * author : Luis Roberto Vega Campos &
 * 			Luis Rubén Padilla Ortiz &
 * 			Arlette Gabriela Uribe Ventura
 * brief  : This module has functions to control driver of DC motor
 */

#include "traction.h"

volatile static float 						g_speed    			= 0;
volatile static bool   						g_auto     			= false;
volatile static speed_sensor_values_t 		g_speed_values   	= {0};
volatile static speed_values_to_send_t      g_values_to_send 	= {0};



void init_traction(FTM_callback_t callback)
{
	CLOCK_EnableClock(PORT_CLOCK_TRACTION);
	PORT_SetPinMux(PWM_PORT_TRACTION, PWM_PIN_TRACTION, PWM_ALTERNATIVE_TRACTION);
	pwm_init(PWM_TIMER_TRACTION, kFTM_Prescale_Divide_4, callback);
	pwm_config(PWM_TIMER_TRACTION, PWM_CHANNEL_TRACTION, kFTM_EdgeAlignedPwm, kFTM_HighTrue, PWM_FRECUENCY_TRACTION, NEUTRAL_PWM_TRACTION);
}

bool set_speed(float speed)
{
	if(MAX_SPEED_TRACTION >= speed && MIN_SPEED_TRACTION <= speed)
	{
		g_auto = true;
		g_speed = speed;
		return true;
	}
	return false;
}

bool set_pwm_traction_time(uint16_t time)
{
	g_auto = false;
	if(MAX_PWM_TRACTION >= time && MIN_PWM_TRACTION <= time)
	{
		pwm_set_time(PWM_TIMER_TRACTION, PWM_CHANNEL_TRACTION, time);
		return true;
	}
	return false;
}


speed_values_to_send_t get_speed()
{
	speed_sensor_values_t speed_values = get_speed_sensor();
	speed_values_to_send_t values_to_send = {
			speed_values.counter,
			g_speed,
			speed_values.speed_m_s,
	};
	return values_to_send;
}

void control_traction_system(float speed)
{
	float error = 0;
	float pwm_out = 0;
	g_speed = speed;
	g_speed_values = get_speed_sensor();
	error = speed-g_speed_values.speed_m_s;
	pwm_out = error * SPEED_TO_PWM_GAIN_TRACTION;
	pwm_out += NEUTRAL_PWM_TRACTION;
	pwm_set_time(PWM_TIMER_TRACTION, PWM_CHANNEL_TRACTION, pwm_out);
}
