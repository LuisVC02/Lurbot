/*
 *	brief :	This module controlls PIT, this has interrupt vectors
 *			and functions for initialize and config time of diferent
 *			timers.
 *
 *	author: Luis Ruben Padilla Ortiz & Luis Roberto Vega Campos
 *	date  : 17-feb-2023
 */

#include "pit.h"

static volatile function_uart g_callbacks[4]    = {0};
static volatile bool     g_pit_initialized = false;
static volatile bool     g_flag_IRQ0       = false;
static volatile bool     g_flag_IRQ1       = false;
static volatile bool     g_flag_IRQ2       = false;
static volatile bool     g_flag_IRQ3       = false;

#define NSEC_TO_COUNT(ns, clockFreqInHz) ((uint32_t)(((uint64_t)(ns) * (clockFreqInHz)) / 1000000000U))

// Initializate -----------------------------------------------------------------------------------------------------------

// module            @ Base address of PIT module
// debug             @ Enable or disable PIT on debug
// timer             @ Timer that will be intialized (of PIT module)
// time_us           @ Starter and load time in us
// enable_interrupts @ Enable or disable interrupt, it doesn't modifies NVIC.
void PIT_Initialization(PIT_Type * module, bool debug, PIT_timer timer, uint32_t time, time_selector_t us_or_ns, bool enable_interrupts)
{
	pit_chnl_t channel = (pit_chnl_t) timer;
	uint32_t timer_period = 0;
	if(false == g_pit_initialized)
	{
		// PIT initialization ---------------------------------------------------------
		pit_config_t PIT_Config = {
				debug //Disable PIT in debug mode
		};

		PIT_Init(module, &PIT_Config); // Initialize module with struct configuration
		g_pit_initialized = true;
	}

	PIT_Chain_mode(module, channel, false);

	// The PIT clock is defined in header file, for select bus clock
	if(u_seconds == us_or_ns)
	{
		timer_period = USEC_TO_COUNT(time, CLOCK_GetFreq(PIT_clock));
	}
	else
	{
		timer_period = NSEC_TO_COUNT(time, CLOCK_GetFreq(PIT_clock));
	}
	PIT_SetTimerPeriod(module, channel, timer_period); // Timer recharge USEC_TO_COUNT(s, clock)

	PIT_ClearStatusFlags(module, channel, Timer_flag);

	if(true == enable_interrupts) PIT_EnableInterrupts(module, channel, kPIT_TimerInterruptEnable);
	else PIT_DisableInterrupts(module, channel, kPIT_TimerInterruptEnable);

	PIT_StartTimer(module, channel); // Starts timer

	// --------------------------------------------------------------------------
}
// -----------------------------------------------------------------------------------------------------------------------

// Stop or start selected timer ------------------------------------------------------------------------------------------
void start_Timer(PIT_Type *base, PIT_timer timer)
{
	pit_chnl_t channel = (pit_chnl_t) timer;
	PIT_StartTimer(base, channel);
}

void stop_Timer(PIT_Type *base, PIT_timer timer)
{
	pit_chnl_t channel = (pit_chnl_t) timer;
	PIT_StopTimer(base, channel);
}
// -----------------------------------------------------------------------------------------------------------------------

// Set time --------------------------------------------------------------------------------------------------------------

// module            @ Base address of PIT module
// timer             @ Timer that will be intialized (of PIT module)
// time_us           @ Load time in us
// time_ms           @ Load time in ms
void setTime(PIT_Type * module , PIT_timer timer, uint32_t time, time_selector_t us_or_ns)
{
	pit_chnl_t channel = (pit_chnl_t) timer;
	uint32_t timer_period = 0;
	if(u_seconds == us_or_ns)
	{
		timer_period = USEC_TO_COUNT(time, CLOCK_GetFreq(PIT_clock));
	}
	else
	{
		timer_period = NSEC_TO_COUNT(time, CLOCK_GetFreq(PIT_clock));
	}
	PIT_SetTimerPeriod(module, channel, timer_period); // Timer recharge USEC_TO_COUNT(s, clock)
}

void setTime_ms(PIT_Type * module , PIT_timer timer, uint32_t time_ms)
{
	pit_chnl_t channel = (pit_chnl_t) timer;
	PIT_SetTimerPeriod(module, channel, USEC_TO_COUNT(time_ms*1000, CLOCK_GetFreq(PIT_clock))); // Timer recharge USEC_TO_COUNT(ms, clock)
}
// -----------------------------------------------------------------------------------------------------------------------

// Sets callback as global function for interrupt ------------------------------------------------------------------------
void PIT_callback_init(PIT_timer timer, function_uart callback)
{
	g_callbacks[timer] = callback;
}
// -----------------------------------------------------------------------------------------------------------------------

// Read_flag of IRQ 3 ----------------------------------------------------------------------------------------------------
bool get_Flag_IRQ0()
{
	bool flag = g_flag_IRQ0;
	g_flag_IRQ0 = false;
	return flag;
}

bool get_Flag_IRQ1()
{
	bool flag = g_flag_IRQ1;
	g_flag_IRQ1 = false;
	return flag;
}

bool get_Flag_IRQ2()
{
	bool flag = g_flag_IRQ2;
	g_flag_IRQ2 = false;
	return flag;
}

bool get_Flag_IRQ3()
{
	bool flag = g_flag_IRQ3;
	g_flag_IRQ3 = false;
	return flag;
}
// -----------------------------------------------------------------------------------------------------------------------

// Interrupts vectors ----------------------------------------------------------------------------------------------------
void PIT0_DriverIRQHandler()// Interrupt vector T0
{
	if(g_callbacks[0])
	{
		g_callbacks[0]();
	}
	g_flag_IRQ0 = true;
	Clear_interrupt_flag(PIT_module, T0, Timer_flag);
}

void PIT1_DriverIRQHandler()// Interrupt vector T1
{
	if(g_callbacks[1])
	{
		g_callbacks[1]();
	}
	g_flag_IRQ1 = true;
	Clear_interrupt_flag(PIT_module, T1, Timer_flag);
}

void PIT2_DriverIRQHandler()// Interrupt vector T2
{
	if(g_callbacks[2])
	{
		g_callbacks[2]();
	}
	g_flag_IRQ2 = true;
	Clear_interrupt_flag(PIT_module, T2, Timer_flag);
}

void PIT3_DriverIRQHandler()// Interrupt vector T3
{
	if(g_callbacks[3])
	{
		g_callbacks[3]();
	}
	g_flag_IRQ3 = true;
	Clear_interrupt_flag(PIT_module, T3, Timer_flag);
}
// -----------------------------------------------------------------------------------------------------------------------
