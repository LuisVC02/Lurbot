/*
 * brief   : This module has a main program of robot
 * date    : 21/April/2023
 * authors :
 * 			Luis Roberto Vega Campos &
 * 			Luis Ruben Padilla Ortiz &
 * 			Arlette Gabriela Uribe Ventura
 */

#include "config.h"
#include "traction.h"
#include "direction.h"
#include "controller.h"
#include "nvic.h"

void control_values(channel_controller_t channel_values);
volatile static traction_t g_traction = {0};
traction_t update_motor();

int main()
{
	CLOCK_SetSimSafeDivs();
	RC_init();
	traction_init();
	set_control_callback(control_values);
	set_callback(update_motor);
	NVIC_enable_interrupt_and_priotity(DMA_CH0_IRQ, PRIORITY_3);
	NVIC_enable_interrupt_and_priotity(PIT_CH0_IRQ, PRIORITY_2);
	NVIC_enable_interrupt_and_priotity(FTM0_IRQ, PRIORITY_4);
	NVIC_global_enable_interrupts;
	while(1)
	{

	}
	return 0;
}


void control_values(channel_controller_t channel_values)
{
	int16_t traction_value = channel_values.motor_H;
	if(true == channel_values.good_comunication)
	{
		traction_value = traction_value << 8;
		traction_value |= channel_values.motor_L;
	}
	else
	{
		traction_value = NEUTRAL_PWM_US;
	}

	direction_traction_t direction_traction = (traction_value < NEUTRAL_PWM_US)? backward_t:forward_t;
	traction_value -= MIN_PWM_PULSE_US;
	traction_value = (direction_traction == forward_t)? (traction_value-MAX_SPEED):(MAX_SPEED-traction_value);
	g_traction.speed = traction_value;
	g_traction.direction = direction_traction;
}


traction_t update_motor()
{
	return g_traction;
}
