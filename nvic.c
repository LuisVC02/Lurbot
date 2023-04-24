/*
 * 	brief :	This module controlls NVIC and is the global interrupt controller
 *	date  : 17/02/2023
 *	author: Luis Ruben Padilla Ortiz & Luis Roberto Vega Campos
 */

#include "nvic.h"

// This function enable specific interrupt and put priority -----------------------------------------
void NVIC_enable_interrupt_and_priotity(interrupt_t interrupt_number, priority_level_t priority)
{
	NVIC_EnableIRQ(interrupt_number);
	NVIC_SetPriority(interrupt_number, priority);
}
// --------------------------------------------------------------------------------------------------

// This function disable specific interrupt ---------------------------------------------------------
void NVIC_disable_interrupt(interrupt_t interrupt_number){
	NVIC_DisableIRQ(interrupt_number);
}
// --------------------------------------------------------------------------------------------------


