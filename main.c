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
#include "telemetry.h"

#define PWM1_PORT PORTC
#define PWM1_PIN  1

#define PWM2_PORT PORTA
#define PWM2_PIN  6

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

	// NVIC intialization ------------------------------------------------------------
	NVIC_enable_interrupt_and_priotity(FTM0_IRQ, PRIORITY_3);
	NVIC_global_enable_interrupts;
	// -------------------------------------------------------------------------------


	while(1)
	{

	}
	return 0;
}
