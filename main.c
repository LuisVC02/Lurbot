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
	CLOCK_EnableClock(kCLOCK_PortC);
	CLOCK_EnableClock(kCLOCK_PortA);
	// -------------------------------------------------------------------------------

	// PORT intialization ------------------------------------------------------------
	PORT_SetPinMux(PWM1_PORT, PWM1_PIN, kPORT_MuxAlt4);
	PORT_SetPinMux(PWM2_PORT, PWM2_PIN, kPORT_MuxAlt3);
	// -------------------------------------------------------------------------------

	// PWM initialization ------------------------------------------------------------
	pwm_init(FlexTimer0, kFTM_Prescale_Divide_4);
	pwm_config(FlexTimer0, kFTM_Chnl_0, kFTM_EdgeAlignedPwm, kFTM_HighTrue, 50, 1500);
	pwm_config(FlexTimer0, kFTM_Chnl_3, kFTM_EdgeAlignedPwm, kFTM_HighTrue, 50, 2000);
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
		pwm_set_time(FlexTimer0, kFTM_Chnl_0, 1000);
	}
	return 0;
}
