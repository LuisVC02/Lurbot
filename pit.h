/*
 *	brief :	This module controlls PIT, this has interrupt vectors
 *			and functions for initialize and config time of diferent
 *			timers.
 *
 *	author: Luis Ruben Padilla Ortiz & Luis Roberto Vega Campos
 *	date  : 17-feb-2023
 */

#ifndef PIT_H_
#define PIT_H_

#include "MK66F18.h"
#include "fsl_pit.h"
#include <stdbool.h>

// Definitions ----------------------------------------------
#define PIT_clock  kCLOCK_BusClk  // Timers clock
#define PIT_module PIT            // Base address of PIT module
#define Timer_flag kPIT_TimerFlag // Interrupt flag
// ----------------------------------------------------------

typedef void(*function_uart)(void);

// Clear flag interrupt -----------------------------------------------------------------

// module            @ Base address of PIT module
// timer             @ Timer (PIT module)
// flag              @ Interruption flag -> Timer_flag (Defined at the begin)
// status            @ Enable or disable interrupts

#define Clear_interrupt_flag(module, timer, flag)           PIT_ClearStatusFlags(module, (pit_chnl_t) timer, flag);

// Enable or disable chain mode in timer ------------------------------------------------
#define PIT_Chain_mode(module, channel, status)             PIT_SetTimerChainMode(module, channel, status);
// --------------------------------------------------------------------------------------

// Timers of PIT module -------------------------------------
typedef enum{
	T0 = kPIT_Chnl_0,
	T1 = kPIT_Chnl_1,
	T2 = kPIT_Chnl_2,
	T3 = kPIT_Chnl_3
}PIT_timer;

typedef enum{
	u_seconds,
	n_seconds
} time_selector_t;
// ----------------------------------------------------------




// Initialize ----------------------------------------------------------------------

// module            @ Base address of PIT module
// debug             @ Enable or disable PIT on debug
// timer             @ Timer that will be initialized (of PIT module)
// time_us           @ Starter and load time in us
// enable_interrupts @ Enable or disable interrupt, it doesn't modifies NVIC.

void PIT_Initialization(PIT_Type * module, bool debug, PIT_timer timer, uint32_t time, time_selector_t us_or_ns, bool enable_interrupts);

// -------------------------------------------------------------------------------------

// Stop or start selected timer --------------------------------------------------------
void stop_Timer(PIT_Type *base, PIT_timer timer);
void start_Timer(PIT_Type *base, PIT_timer timer);
// -------------------------------------------------------------------------------------

// Set time -----------------------------------------------------------------------------

// module            @ Base address of PIT module
// timer             @ Timer that will be initialized (of PIT module)
// time_us           @ Load time in us
// time_ms           @ Load time in ms

void setTime(PIT_Type * module , PIT_timer timer, uint32_t time, time_selector_t us_or_ns);
void setTime_ms(PIT_Type * module , PIT_timer timer, uint32_t time_ms);
// --------------------------------------------------------------------------------------

// Sets callback as global function for interrupt ---------------------------------------
void PIT_callback_init(PIT_timer timer, function_uart callback);
// --------------------------------------------------------------------------------------

// Read_flag of IRQ 3 ----------------------------------------
bool get_Flag_IRQ0();
bool get_Flag_IRQ1();
bool get_Flag_IRQ2();
bool get_Flag_IRQ3();
// -----------------------------------------------------------

// Interrupts vectors ----------------------------------------
void PIT0_DriverIRQHandler(void); // Interrupt vector T0
void PIT1_DriverIRQHandler(void); // Interrupt vector T1
void PIT2_DriverIRQHandler(void); // Interrupt vector T2
void PIT3_DriverIRQHandler(void); // Interrupt vector T3
// ----------------------------------------------------------

#endif /* PIT_H_ */
