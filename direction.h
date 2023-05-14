/*
 * brief   : This module has direction functions
 * date    : 21/April/2023
 * authors :
 * 			Luis Roberto Vega Campos &
 * 			Luis Ruben Padilla Ortiz &
 * 			Arlette Gabriela Uribe Ventura
 */

#ifndef DIRECTION_H_
#define DIRECTION_H_

#include <stdint.h>
#include <stdbool.h>
#include "fsl_ftm.h"
#include "fsl_port.h"
#include "fsl_clock.h"
#include "traction.h"

#define FLEX_TIMER_TO_USE_DIRECTION  FTM0
#define FLEX_TIMER_CHANNEL_DIRECTION kFTM_Chnl_3

#define PWM_FRECUENCY_DIRECTION      50u

#define PORT_PWM_CH3                 PORTA
#define PIN_PWM_CH3                  6u
#define PORT_ALTERNATIVE_DIRECTION   kPORT_MuxAlt3
#define PORT_CLOCK_DIRECTION         kCLOCK_PortA

#define MAX_PWM_DIRECTION            2000
#define MIN_PWM_DIRECTION            1000
#define NEUTRAL_PWM_DIRECTION        1500

#define MAX_ANGLE                   90
#define MIN_ANGLE                   -90

#define OFFSET_DIRECTION            -120

typedef int8_t(*function_direction_t)(void);

void direction_init();

// Values of -90 to 90
void set_direction(int16_t direction);

int16_t get_direction();

// CALLBACK IS THE SAME AS TRACTION -------------------


#endif /* DIRECTION_H_ */
