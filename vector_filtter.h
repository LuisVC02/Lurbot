/*
 * date   : 12/June/2023
 * author : Luis Roberto Vega Campos &
 * 			Luis Rubén Padilla Ortiz &
 * 			Arlette Gabriela Uribe Ventura
 * brief  :
 */

#ifndef VECTOR_FILTTER_H_
#define VECTOR_FILTTER_H_

#include "pixy2LineTracking.h"
#include "stdbool.h"

typedef uint8_t angle_t;
typedef uint8_t slope_t;

typedef struct _direction
{
	angle_t angle;
	slope_t slope;
}direction_t;


void getDirectionVecs(vector_t * vecsBuff, uint8_t vecsLen, direction_t * directionBuff);

void filtSlope(
		slope_t * 	slopeBuff,
		uint8_t 	slopeBuffLen,
		uint64_t * 	maskSlope,
		slope_t 	maxSlope,
		slope_t 	minSlope
		);

#endif /* VECTOR_FILTTER_H_ */
