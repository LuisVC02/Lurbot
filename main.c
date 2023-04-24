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
volatile static int16_t g_angle = 0;
void update_motor_and_direction();

int main()
{
	CLOCK_SetSimSafeDivs();
	RC_init();
	init_pwm();
	traction_init();
	direction_init();
	set_control_callback(control_values);
	set_callback(update_motor_and_direction);
	NVIC_enable_interrupt_and_priotity(DMA_CH0_IRQ, PRIORITY_4);
	NVIC_enable_interrupt_and_priotity(PIT_CH0_IRQ, PRIORITY_4);
	NVIC_enable_interrupt_and_priotity(FTM0_IRQ, PRIORITY_2);
	NVIC_global_enable_interrupts;
	while(1)
	{

	}
	return 0;
}


void control_values(channel_controller_t channel_values)
{
	int16_t traction_value = channel_values.motor_H;
	int16_t direction_value = channel_values.direction_H;

	if(true == channel_values.good_comunication)
	{
		traction_value = traction_value << 8;
		direction_value = direction_value << 8;
		traction_value |= channel_values.motor_L;
		direction_value |= channel_values.direction_L;
	}
	else
	{
		traction_value = NEUTRAL_PWM_US;
		direction_value = NEUTRAL_PWM_DIRECTION;
	}

	direction_traction_t direction_traction = (traction_value < NEUTRAL_PWM_US)? backward_t:forward_t;
	traction_value -= MIN_PWM_PULSE_US;
	traction_value = (direction_traction == forward_t)? (traction_value-MAX_SPEED):(MAX_SPEED-traction_value);
	g_traction.speed = traction_value;
	g_traction.direction = direction_traction;
	g_angle = (direction_value - NEUTRAL_PWM_DIRECTION)/-5;
}


void update_motor_and_direction()
{
	set_direction(g_angle);
	set_traction(g_traction);
}
