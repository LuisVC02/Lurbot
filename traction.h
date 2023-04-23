/*
 * brief   : This module has traction functions
 * date    : 21/April/2023
 * authors :
 * 			Luis Roberto Vega Campos &
 * 			Luis Ruben Padilla Ortiz &
 * 			Arlette Gabriela Uribe Ventura
 */

#ifndef TRACTION_H_
#define TRACTION_H_

#include "fsl_ftm.h"
#include "fsl_port.h"
#include "fsl_clock.h"

#define FLEX_TIMER_TO_USE  FTM0
#define FLEX_TIMER_CHANNEL kFTM_Chnl_0

#define PORT_PWM_CH1       PORTC
#define PIN_PWM_CH1        1u
#define PORT_ALTERNATIVE   kPORT_MuxAlt4
#define PORT_CLOCK         kCLOCK_PortC

#define PWM_FRECUENCY      50u
#define MAX_PWM_PULSE_US   2000u
#define NEUTRAL_PWM_US     1500u
#define MIN_PWM_PULSE_US   1000u
#define MAX_SPEED          500u

typedef enum
{
	forward_t,
	backward_t
} direction_traction_t;

typedef struct
{
	direction_traction_t direction;
	int16_t             speed;
}traction_t;

void traction_init();

void set_speed(traction_t traction);


void add_speed(traction_t traction);

#endif /* TRACTION_H_ */
