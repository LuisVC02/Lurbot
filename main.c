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
#include "line_sensor.h"
#include "Pixy2.h"
#include "Pixy2LineTracking.h"

#define SPEED_AUTO 300
#define ANGLE_LEFT_1_AUTO  20
#define ANGLE_LEFT_2_AUTO  60
#define ANGLE_RIGHT_1_AUTO -20
#define ANGLE_RIGHT_2_AUTO -60
#define ANGLE_NEUTRAL_AUTO 0

// For camera control --------------------
#define CENTRAL_POSITION   39
// ---------------------------------------

typedef enum
{
	manual_mode_st,
	auto_mode_st
} control_mode_t;

volatile static traction_t     g_traction     = {0};
volatile static int16_t        g_angle        = 0;
volatile static control_mode_t g_control_mode = manual_mode_st;

void control_values(channel_controller_t channel_values);
void update_motor_and_direction();
void manual_mode_func();
void auto_mode_func();

int main()
{
	CLOCK_SetSimSafeDivs();
	RC_init();
	init_pwm();
	traction_init();
	direction_init();
	set_control_callback(control_values);
	set_callback(update_motor_and_direction);
	Init_Pixy2();
	NVIC_enable_interrupt_and_priotity(DMA_CH0_IRQ, PRIORITY_3);
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
	int16_t sw_mode_value = channel_values.sw2_H;
	int16_t sw_auto_value = channel_values.sw1_H;

	if(true == channel_values.good_comunication)
	{
		traction_value = traction_value << 8;
		direction_value = direction_value << 8;
		sw_mode_value = sw_mode_value << 8;
		sw_auto_value = sw_auto_value << 8;
		traction_value |= channel_values.motor_L;
		direction_value |= channel_values.direction_L;
		sw_mode_value |= channel_values.sw2_L;
		sw_auto_value |= channel_values.sw1_L;
	}
	else
	{
		traction_value = NEUTRAL_PWM_US;
		direction_value = NEUTRAL_PWM_DIRECTION;
		sw_auto_value = MIN_PWM_PULSE_US;
	}
	if(NEUTRAL_PWM_US == sw_mode_value)
	{
		set_normal_traction();
	}
	else if(NEUTRAL_PWM_US < sw_mode_value)
	{
		set_sport_traction();
	}
	else
	{
		set_slow_traction();
	}

	if(NEUTRAL_PWM_US > sw_auto_value)
	{
		g_control_mode = manual_mode_st;
	}
	else
	{
		g_control_mode = auto_mode_st;
	}

	if(manual_mode_st == g_control_mode)
	{
		direction_traction_t direction_traction = (traction_value < NEUTRAL_PWM_US)? backward_t:forward_t;
		traction_value -= MIN_PWM_PULSE_US;
		traction_value = (direction_traction == forward_t)? (traction_value-MAX_SPEED):(MAX_SPEED-traction_value);
		g_traction.speed = traction_value;
		g_traction.direction = direction_traction;
		g_angle = (direction_value - NEUTRAL_PWM_DIRECTION)/-5;
	}
	else
	{
		auto_mode_func();
	}
}


void update_motor_and_direction()
{
	set_direction(g_angle);
	set_traction(g_traction);
}

void manual_mode_func()
{

}

void auto_mode_func()
{
	uint8_t elements = 0;
	uint8_t index    = 0;
	featureTypeBuff_t *camera_data = getMainFeatures_LinePixy2();
	elements = camera_data->featureLen;
	vector_t *vectors = (vector_t*)&camera_data->featureData;

	uint8_t firstVector = 0;
	uint8_t secondVector = 0;

	for(index = 0; index < elements; index ++)
	{
		if(1 == vectors[index].m_index)
		{
			firstVector = (vectors[index].m_x0 + vectors[index].m_x1)/2;
		}
		else if(2 == vectors[index].m_index)
		{
			secondVector = (vectors[index].m_x0 + vectors[index].m_x1)/2;
		}
	}
	uint8_t central_position = (firstVector + secondVector)/2;
	int8_t  diference        = central_position-CENTRAL_POSITION;
	g_angle = diference*2;
	g_traction.direction = forward_t;
	g_traction.speed = SPEED_AUTO;
}
