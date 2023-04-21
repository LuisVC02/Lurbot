/*
 * brief   : This module has traction functions
 * date    : 21/April/2023
 * authors :
 * 			Luis Roberto Vega Campos &
 * 			Luis Ruben Padilla Ortiz &
 * 			Arlette Gabriela Uribe Ventura
 */

#include "traction.h"


void traction_init()
{
	const ftm_config_t config =
	{

	};

	CLOCK_EnableClock(PORT_CLOCK);
	PORT_SetPinMux(PORT_PWM_CH1, PIN_PWM_CH1, PORT_ALTERNATIVE);


	FTM_Init(FLEX_TIMER_TO_USE, &config);
}

void set_speed(traction_t traction)
{

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

