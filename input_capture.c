/*
 * date   : 15/June/2023
 * author : Luis Roberto Vega Campos &
 * 			Luis Rubén Padilla Ortiz &
 * 			Arlette Gabriela Uribe Ventura
 * brief  :
 */
#include "input_capture.h"

volatile static uint32_t g_clock_frec                            =  1;
volatile static uint32_t g_prescaler[N_TIMERS_IC]                = {0};
volatile static bool     g_init[N_TIMERS_IC]                     = {0};
volatile static bool     g_updated[N_TIMERS_IC][N_CHANNELS_IC]   = {0};
volatile static int32_t g_max_count[N_TIMERS_IC][N_CHANNELS_IC]  = {0};
volatile static int32_t g_last_value[N_TIMERS_IC][N_CHANNELS_IC] = {0};

volatile static uint32_t g_input_capture[N_TIMERS_IC][N_CHANNELS_IC]    = {0};


const FTM_Type * g_flex_timers_ic[N_TIMERS_IC] =
{
		FTM0,
		FTM1,
		FTM2,
		FTM3
};

void callback_dma(struct _edma_handle *handle, void *userData, bool transferDone, uint32_t tcds);

void init_input_capture(flex_timer_ic_t timer, ftm_chnl_t channel, ftm_clock_prescale_t prescaler)
{
	static FTM_Type * base;
	static ftm_config_t config;
	base = (FTM_Type*)g_flex_timers_ic[timer];
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
		// --------------------------------------------------------

		// Enables Flex timer -------------------------------------
		FTM_Init(base, &config);
		FTM_EnableInterrupts(base, kFTM_Chnl0InterruptEnable|kFTM_TimeOverflowInterruptEnable);
		// --------------------------------------------------------

		g_init[timer] = true;

	}
	// -----------------------------------------------------------------
}

void config_input_capture(flex_timer_ic_t timer, ftm_chnl_t chnlNumber, ftm_input_capture_edge_t captureMode, uint32_t filterValue, uint32_t time_us)
{
	static FTM_Type * base;
	base = (FTM_Type*)g_flex_timers_ic[timer];
	uint16_t count = USEC_TO_COUNT(time_us, g_clock_frec/g_prescaler[timer]);
	g_max_count[timer][chnlNumber] = count;

	FTM_StopTimer(base);
	FTM_SetupInputCapture(base, chnlNumber, captureMode, filterValue);
	FTM_SetTimerPeriod(base, count);
	FTM_StartTimer(base, kFTM_SystemClock);
}

uint32_t get_input_capture_value(flex_timer_ic_t timer, ftm_chnl_t chnlNumber)
{
	return g_input_capture[timer][chnlNumber];
}

/*
void FTM0_IRQHandler()
{
	uint32_t status_flags = FTM_GetStatusFlags(FTM0);
	FTM_ClearStatusFlags(FTM0, kFTM_TimeOverflowFlag);
}


void FTM1_IRQHandler()
{
	uint32_t status_flags = FTM_GetStatusFlags(FTM1);
	FTM_ClearStatusFlags(FTM1, kFTM_TimeOverflowFlag);
}
*/

void FTM2_IRQHandler()
{
	uint32_t status_flags = FTM_GetStatusFlags(FTM2);
	int32_t result       = 0;
	int32_t new_value    = 0;
	int32_t last_value   = g_last_value[FlexTimer2_IC][0];
	int32_t max_count    = g_max_count[FlexTimer2_IC][0];
	if(kFTM_TimeOverflowFlag == (status_flags & kFTM_TimeOverflowFlag))
	{
		g_input_capture[FlexTimer2_IC][0] = 0;
		FTM_ClearStatusFlags(FTM2, kFTM_TimeOverflowFlag);
	}
	if(kFTM_Chnl0Flag == (status_flags & kFTM_Chnl0Flag))
	{
		new_value = FTM_GetInputCaptureValue(FTM2, 0);
		result = new_value - last_value;
		result = (result < 0)? (max_count+result):result;
		g_last_value[FlexTimer2_IC][0] = new_value;
		if(result > 200)
		{
			g_input_capture[FlexTimer2_IC][0] = result;
		}
		//telemetry_send_unblocking(2, (uint8_t*)&g_input_capture[FlexTimer2_IC][0]);
		FTM_ClearStatusFlags(FTM2, kFTM_Chnl0Flag);
	}
}

/*
void FTM3_IRQHandler()
{
	uint32_t status_flags = FTM_GetStatusFlags(FTM3);
	FTM_ClearStatusFlags(FTM3, kFTM_TimeOverflowFlag);
}
*/

