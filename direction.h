/*
 * date   : 12/June/2023
 * author : Luis Roberto Vega Campos &
 * 			Luis Rubén Padilla Ortiz &
 * 			Arlette Gabriela Uribe Ventura
 * brief  :
 */

#ifndef DIRECTION_H_
#define DIRECTION_H_

#include "fsl_port.h"
#include "fsl_clock.h"
#include "pwm_controller.h"
#include <stdint.h>
#include <stdbool.h>

#define MAX_ANGLE_DIRECTION 60   // In km/hr
#define MIN_ANGLE_DIRECTION -60  // In km/hr

#define ANGLE_TO_PWM_GAIN_DIRECTION 6
#define MIN_PWM_DIRECTION           1000
#define NEUTRAL_PWM_DIRECTION       1500
#define MAX_PWM_DIRECTION           2000
#define PWM_FRECUENCY_DIRECTION     50

#define PWM_TIMER_DIRECTION       FlexTimer0
#define PWM_CHANNEL_DIRECTION     kFTM_Chnl_3
#define PWM_PORT_DIRECTION        PORTA
#define PWM_PIN_DIRECTION         6
#define PWM_ALTERNATIVE_DIRECTION kPORT_MuxAlt3
#define PORT_CLOCK_DIRECTION      kCLOCK_PortA

void init_direction();

bool set_angle(int8_t angle);

bool set_pwm_direction_time(uint16_t time);

int8_t get_angle();

#endif /* DIRECTION_H_ */
