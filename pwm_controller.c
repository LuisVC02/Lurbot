/*
 * date   : 12/June/2023
 * author : Luis Roberto Vega Campos &
 * 			Luis Rubén Padilla Ortiz &
 * 			Arlette Gabriela Uribe Ventura
 * brief  :
 */

#include "pwm_controller.h"
#include <stdbool.h>


const FTM_Type * g_flex_timers_pwm[N_TIMERS] =
{
		FTM0,
		FTM1,
		FTM2,
		FTM3
};

volatile static FTM_callback_t g_callback[N_TIMERS]           = {NULL};

volatile static uint32_t  g_clock_frec =  1;
volatile static uint32_t  g_timer_frec[N_TIMERS] =  {0};
volatile static uint32_t  g_prescaler[N_TIMERS]  =  {0};
volatile static bool      g_init[N_TIMERS]       =  {0};

volatile static uint32_t  g_new_time[N_TIMERS][N_CHANNELS]    = {0};
volatile static bool      g_time_change[N_TIMERS][N_CHANNELS] = {0};
volatile static uint32_t *g_new_time_flex0_ch0                =  0;
volatile static uint32_t *g_new_time_flex0_ch3                =  0;





void pwm_init(flex_timer_pwm_t timer, ftm_clock_prescale_t prescaler, FTM_callback_t callback)
{
	static FTM_Type * base;
	static ftm_config_t config;
	base = (FTM_Type*)g_flex_timers_pwm[timer];
	// Check if timer was initialized ----------------------------------
	if(false == g_init[timer])
	{
		// Get default configurations and clock frecuency ---------
		g_clock_frec = CLOCK_GetFreq(kCLOCK_BusClk);
		FTM_GetDefaultConfig(&config);
		// --------------------------------------------------------

		// Configurations -----------------------------------------
		config.prescale = prescaler;
		g_prescaler[timer] = 1 << prescaler;
		config.bdmMode = kFTM_BdmMode_3;
		// --------------------------------------------------------

		// Internal initializations -------------------------------
		g_new_time_flex0_ch0 = (uint32_t*)&g_new_time[FlexTimer0_PWM][0];
		g_new_time_flex0_ch3 = (uint32_t*)&g_new_time[FlexTimer0_PWM][3];
		g_timer_frec[timer] = g_clock_frec/g_prescaler[timer];
		g_callback[timer] = callback;
		// --------------------------------------------------------

		// Enables Flex timer -------------------------------------
		FTM_Init(base, &config);
		FTM_EnableInterrupts(base, kFTM_TimeOverflowInterruptEnable);
		FTM_StartTimer(base, kFTM_SystemClock);
		// --------------------------------------------------------
		g_init[timer] = true;
	}
	// -----------------------------------------------------------------
}

void pwm_config(flex_timer_pwm_t timer, ftm_chnl_t channel, ftm_pwm_mode_t pwm_mode, ftm_pwm_level_select_t level, uint32_t frecuency, uint32_t time_us)
{
	FTM_Type * base = (FTM_Type*)g_flex_timers_pwm[timer];
	ftm_chnl_pwm_signal_param_t pwm_config =
	{
			channel,
			level,
			50u,
			0u,
			false,
			false
	};

	FTM_StopTimer(base);
	FTM_SetupPwm(base, &pwm_config, 1u, pwm_mode, frecuency, g_clock_frec);
	base->CONTROLS[channel].CnV = USEC_TO_COUNT(time_us, g_clock_frec/g_prescaler[timer]);
	FTM_SetSoftwareTrigger(base, true);
	FTM_StartTimer(base, kFTM_SystemClock);
}

void pwm_set_time(flex_timer_pwm_t timer, ftm_chnl_t channel, uint32_t time_us)
{
	if(g_new_time[timer][channel] != time_us)
	{
		g_new_time[timer][channel]    = time_us;
		g_time_change[timer][channel] = true;
	}
}

void pwm_set(flex_timer_pwm_t timer, ftm_chnl_t channel, uint32_t time_us)
{
	FTM_Type * base = (FTM_Type*)g_flex_timers_pwm[timer];

	FTM_StopTimer(base);
	base->CONTROLS[channel].CnV = USEC_TO_COUNT(time_us, g_clock_frec/g_prescaler[timer]);
	FTM_SetSoftwareTrigger(base, true);
	FTM_StartTimer(base, kFTM_SystemClock);
}

void update_channels(flex_timer_pwm_t timer)
{
	uint8_t channel = 0;

	uint32_t *time_change = (uint32_t*)g_new_time[timer];
	bool *new_channel_value = (bool*)g_time_change[timer];

	for(channel = 0; N_CHANNELS > channel; channel ++)
	{
		if(true == new_channel_value[channel])
		{
			pwm_set(timer, channel, time_change[channel]);
		}
	}
}

void FTM0_IRQHandler()
{
	FTM_Type * base = FTM0;

	// Stop timer -------------------------------------------------------------------------
	base->SC &= 0xE7U;
	// ------------------------------------------------------------------------------------
	// Put new times in channel value -----------------------------------------------------
	base->CONTROLS[0].CnV = USEC_TO_COUNT(*g_new_time_flex0_ch0, g_timer_frec[0]);
	base->CONTROLS[3].CnV = USEC_TO_COUNT(*g_new_time_flex0_ch3, g_timer_frec[0]);
	// ------------------------------------------------------------------------------------
	// Software trigger -------------------------------------------------------------------
	base->SYNC |= FTM_SYNC_SWSYNC_MASK;
	// ------------------------------------------------------------------------------------
	// Start timer ------------------------------------------------------------------------
	base->SC |= 0x08U;
	// ------------------------------------------------------------------------------------

	// Callbacks --------------------------------------------------------------------------
	g_callback[FlexTimer0_PWM]();
	// ------------------------------------------------------------------------------------

	// Clear overflow ---------------------------------------------------------------------
	base->SC &= ~FTM_SC_TOF_MASK;
	// ------------------------------------------------------------------------------------
}

/*
void FTM1_IRQHandler()
{
	update_channels(FlexTimer1_PWM);
	FTM_ClearStatusFlags(FTM1, kFTM_TimeOverflowFlag);
}

void FTM2_IRQHandler()
{
	update_channels(FlexTimer2_PWM);
	FTM_ClearStatusFlags(FTM2, kFTM_TimeOverflowFlag);
}

void FTM3_IRQHandler()
{
	update_channels(FlexTimer3_PWM);
	FTM_ClearStatusFlags(FTM3, kFTM_TimeOverflowFlag);
}
*/
