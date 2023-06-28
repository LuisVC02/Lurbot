/*
 * date   : 12/June/2023
 * author : Luis Roberto Vega Campos &
 * 			Luis Rubén Padilla Ortiz &
 * 			Arlette Gabriela Uribe Ventura
 * brief  :
 */

#ifndef SPEED_SENSOR_H_
#define SPEED_SENSOR_H_

#include "input_capture.h"
#include "fsl_port.h"
#include "fsl_clock.h"

#define SPEED_SENSOR_PORT        PORTA
#define SPEED_SENSOR_PIN         10
#define SPEED_SENSOR_ALTERNATIVE kPORT_MuxAlt3
#define SPEED_SENSOR_PORT_CLOCK  kCLOCK_PortA

#define SPEED_SENSOR_FTM       FlexTimer2_IC
#define SPEED_SENSOR_CHANNEL   kFTM_Chnl_0
#define SPEED_SENSOR_PRESCALER kFTM_Prescale_Divide_32
#define FTM_FRECUENCY          218750.0

typedef struct _speed_sensor_values_t
{
	uint16_t counter;
	float rpm_diff;
	float rpm_whole;
	float speed_m_s;
	float speed_km_hr;
}speed_sensor_values_t;

void init_speed_sensor();

speed_sensor_values_t get_speed_sensor();

#endif /* SPEED_SENSOR_H_ */
