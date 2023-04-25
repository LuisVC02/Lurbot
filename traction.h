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

#include <stdint.h>
#include <stdbool.h>
#include "fsl_ftm.h"
#include "fsl_port.h"
#include "fsl_clock.h"

#define timeus_to_duty(time) (100*time)/(1000000/PWM_FRECUENCY)

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
#define OFFSET             -150

#define DIVIDER_SPORT      1u
#define DIVIDER_NORMAL     2u
#define DIVIDER_SLOW       4u

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

typedef void(*function_traction_t)(void);

void init_pwm();

void traction_init();

void set_traction(traction_t traction);

traction_t get_traction();

void set_sport_traction();

void set_normal_traction();

void set_slow_traction();

void set_callback(function_traction_t callback);

#endif /* TRACTION_H_ */
