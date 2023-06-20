/*
 * date   : 12/June/2023
 * author : Luis Roberto Vega Campos &
 * 			Luis Rubén Padilla Ortiz &
 * 			Arlette Gabriela Uribe Ventura
 * brief  : This module has functions to control driver of DC motor
 */

#include "traction.h"

volatile static float g_speed                          = 0;
volatile static bool   g_auto                          = false;
volatile static speed_sensor_values_t g_speed_values   = {0};
volatile static values_to_send_t      g_values_to_send = {0};

void control_system();

void init_traction()
{
	CLOCK_EnableClock(PORT_CLOCK_TRACTION);
	PORT_SetPinMux(PWM_PORT_TRACTION, PWM_PIN_TRACTION, PWM_ALTERNATIVE_TRACTION);
	pwm_init(PWM_TIMER_TRACTION, kFTM_Prescale_Divide_4);
	pwm_config(PWM_TIMER_TRACTION, PWM_CHANNEL_TRACTION, kFTM_EdgeAlignedPwm, kFTM_HighTrue, PWM_FRECUENCY_TRACTION, NEUTRAL_PWM_TRACTION);
	PIT_Initialization(PIT, false, kPIT_Chnl_2, 20000, u_seconds, true);
	PIT_callback_init(kPIT_Chnl_2, control_system);
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


int8_t get_speed()
{
	return g_speed;
}

void control_system()
{
	float error = 0;
	float pwm_out = 0;
	g_speed_values = get_speed_sensor();
	g_values_to_send.ftm_count = g_speed_values.counter;
	g_values_to_send.speed_sensor_x1000 = (uint32_t)(g_speed_values.speed_km_hr+1000.0);
	g_values_to_send.speed_x1000 = (uint32_t)(g_speed+1000.0);
	if(true == g_auto)
	{
		error = g_speed-g_speed_values.speed_km_hr;
		pwm_out = error * SPEED_TO_PWM_GAIN_TRACTION;
		pwm_out += NEUTRAL_PWM_TRACTION;
		pwm_set_time(PWM_TIMER_TRACTION, PWM_CHANNEL_TRACTION, pwm_out);
	}
	telemetry_send_unblocking(12, (uint8_t*)&g_values_to_send);
}
