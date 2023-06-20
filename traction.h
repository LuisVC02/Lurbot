/*
 * date   : 12/June/2023
 * author : Luis Roberto Vega Campos &
 * 			Luis Rubén Padilla Ortiz &
 * 			Arlette Gabriela Uribe Ventura
 * brief  :
 */

#ifndef TRACTION_H_
#define TRACTION_H_

#include "fsl_port.h"
#include "fsl_clock.h"
#include "pwm_controller.h"
#include "speed_sensor.h"
#include "telemetry.h"
#include "pit.h"
#include <stdint.h>
#include <stdbool.h>

#define MAX_SPEED_TRACTION 50   // In km/hr
#define MIN_SPEED_TRACTION -30  // In km/hr

#define SPEED_TO_PWM_GAIN_TRACTION 30.0
#define MIN_PWM_TRACTION           1000
#define NEUTRAL_PWM_TRACTION       1500
#define MAX_PWM_TRACTION           2000
#define PWM_FRECUENCY_TRACTION     50

#define PWM_TIMER_TRACTION       FlexTimer0_PWM
#define PWM_CHANNEL_TRACTION     kFTM_Chnl_0
#define PWM_PORT_TRACTION        PORTC
#define PWM_PIN_TRACTION         1
#define PWM_ALTERNATIVE_TRACTION kPORT_MuxAlt4
#define PORT_CLOCK_TRACTION      kCLOCK_PortC

typedef struct _values_to_send_t
{
	uint32_t ftm_count;
	uint32_t speed_x1000;
	uint32_t speed_sensor_x1000;
} values_to_send_t;

void init_traction();

bool set_speed(float speed);

bool set_pwm_traction_time(uint16_t time);

int8_t get_speed();

#endif /* TRACTION_H_ */
