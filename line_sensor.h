/*
 * brief   : This module has line sensor functions
 * date    : 11/May/2023
 * authors :
 * 			Luis Roberto Vega Campos &
 * 			Luis Ruben Padilla Ortiz
 */

#ifndef LINE_SENSOR_H_
#define LINE_SENSOR_H_

#include "i2c.h"
#include "gpio.h"


void init_sensor();

uint8_t get_sensorValues();

#endif /* LINE_SENSOR_H_ */
