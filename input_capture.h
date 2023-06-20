/*
 * input_compare_controller.h
 *
 *  Created on: 15 jun 2023
 *      Author: luisr
 */

#ifndef INPUT_COMPARE_CONTROLLER_H_
#define INPUT_COMPARE_CONTROLLER_H_

#include "fsl_ftm.h"
#include "dma.h"
#include "telemetry.h"

#define N_TIMERS_IC    4
#define N_CHANNELS_IC  8

#define DMA_CHANNEL_IC 1

typedef enum
{
	FlexTimer0_IC,
	FlexTimer1_IC,
	FlexTimer2_IC,
	FlexTimer3_IC
}flex_timer_ic_t;

void init_input_capture(flex_timer_ic_t timer, ftm_chnl_t channel, ftm_clock_prescale_t prescaler);

void config_input_capture(flex_timer_ic_t timer, ftm_chnl_t chnlNumber, ftm_input_capture_edge_t captureMode, uint32_t filterValue, uint32_t time_us);

uint32_t get_input_capture_value(flex_timer_ic_t timer, ftm_chnl_t chnlNumber);

#endif /* INPUT_COMPARE_CONTROLLER_H_ */
