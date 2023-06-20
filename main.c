/*
 * date   : 12/June/2023
 * author : Luis Roberto Vega Campos &
 * 			Luis Rubén Padilla Ortiz &
 * 			Arlette Gabriela Uribe Ventura
 * brief  :
 */

#include <stdint.h>
#include <stdbool.h>
#include "fsl_clock.h"
#include "fsl_port.h"
#include "gpio.h"
#include "nvic.h"
#include "control_system.h"
#include "traction.h"
#include "direction.h"
#include "rc_control.h"
#include "speed_sensor.h"
#include "telemetry.h"
#include "pit.h"
#include "uart.h"

#define AUTO_MODE_CONTROL_VALUE   2000
#define MANUAL_MODE_CONTROL_VALUE 1000

#define SLOW_MODE_CONTROL_VALUE   1000
#define NORMAL_MODE_CONTROL_VALUE 1500
#define FAST_MODE_CONTROL_VALUE   2000

void automatic_mode();
void manual_mode();
void transfer_data();

volatile static channel_controller_t g_control_values;
volatile static uint8_t              g_speed_divisor   = 1;


int main()
{
	// CLOCK initialization ----------------------------------------------------------
	CLOCK_SetSimSafeDivs();
	// -------------------------------------------------------------------------------

	// Traction initialization -------------------------------------------------------
	init_traction();
	// -------------------------------------------------------------------------------

	// Direction initialization ------------------------------------------------------
	init_direction();
	// -------------------------------------------------------------------------------

	// RC initialization -------------------------------------------------------------
	RC_init();
	// -------------------------------------------------------------------------------

	// Speed sensor initialization ---------------------------------------------------
	init_speed_sensor();
	// -------------------------------------------------------------------------------

	// TELEMETRY initialization ------------------------------------------------------
	telemetry_init();
	// -------------------------------------------------------------------------------

	// NVIC intialization ------------------------------------------------------------
	NVIC_enable_interrupt_and_priotity(DMA_CH0_IRQ, PRIORITY_3);
	NVIC_enable_interrupt_and_priotity(PIT_CH0_IRQ, PRIORITY_4);
	NVIC_enable_interrupt_and_priotity(FTM0_IRQ, PRIORITY_2);
	NVIC_enable_interrupt_and_priotity(FTM2_IRQ, PRIORITY_5);
	NVIC_enable_interrupt_and_priotity(UART4_IRQ, PRIORITY_5);
	NVIC_enable_interrupt_and_priotity(PIT_CH2_IRQ, PRIORITY_6);
	NVIC_global_enable_interrupts;
	// -------------------------------------------------------------------------------


	while(1)
	{
		g_control_values = get_contol_values();

		uint16_t speed_mode = g_control_values.sw2_H << 8;
		speed_mode |= g_control_values.sw2_L;

		uint16_t mode = g_control_values.sw1_H << 8;
		mode |= g_control_values.sw1_L;

		if(true == g_control_values.good_comunication)
		{
			// States machine for auto and manual mode --------------
			if(MANUAL_MODE_CONTROL_VALUE == mode)
			{
				manual_mode();
			}
			else
			{
				automatic_mode();
			}
			// ------------------------------------------------------

			// Selector of speed mode -------------------------------
			if(SLOW_MODE_CONTROL_VALUE == speed_mode)
			{
				g_speed_divisor = 3;
			}
			else if(NORMAL_MODE_CONTROL_VALUE == speed_mode)
			{
				g_speed_divisor = 2;
			}
			else
			{
				g_speed_divisor = 1;
			}
			// ------------------------------------------------------
		}
		else
		{
			set_speed(0);
			set_angle(0);
		}

	}
	return 0;
}

void automatic_mode()
{
	set_speed(4);
	set_angle(0);
}

void manual_mode()
{
	int16_t speed = g_control_values.motor_H << 8;
	speed |= g_control_values.motor_L;
	speed -= 1500;
	speed /= g_speed_divisor;
	speed += 1500;

	uint16_t angle = g_control_values.direction_H << 8;
	angle |= g_control_values.direction_L;

	angle = 3000 - angle;

	set_pwm_traction_time(speed);
	set_pwm_direction_time(angle);
}

void transfer_data()
{
	//telemetry_send_unblocking(2, (uint8_t*)&g_speed_values);
}
