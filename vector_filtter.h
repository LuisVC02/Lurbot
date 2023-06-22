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

#define SCREEN_SIDE_BIT_NUM 	0
#define SCREEN_HEIGHT_BIT_NUM 	1

typedef enum
{
	RightSide,
	LeftSide
}screenSide_t;

typedef enum
{
	top,
	bottom
}screenHeight_t;


typedef int8_t angle_t;
typedef int8_t slope_t;

typedef struct _direction
{
	angle_t angle;
	slope_t slope;
}direction_t;


static inline bool validateRangeSlpoe(slope_t max, slope_t min, slope_t slopeToValidate)
{
	bool retval = true;
	if(slopeToValidate > max)
	{
		retval = false;
	}

	if(slopeToValidate < min)
	{
		retval = false;
	}

	return retval;
}

static inline direction_t calculateDirection(vector_t vector)
{
	direction_t retDirection = {0};
	retDirection.angle = 0;
	if(0 == vector.m_x1 - vector.m_x0)
	{
		vector.m_x1++;
	}
	retDirection.slope = (vector.m_y1 - vector.m_y0)/(vector.m_x1 - vector.m_x0);
	return retDirection;
}

static inline void swapVector(vector_t* vec)
{
	vector_t vecTemp = *vec;


}


void getDirectionVecs(vector_t * vecsBuff, uint8_t vecsLen, direction_t * directionBuff);

void filtSlope(
		slope_t * 	slopeBuff,
		uint8_t 	slopeBuffLen,
		uint64_t * 	maskSlope,
		slope_t 	maxSlope,
		slope_t 	minSlope
		);

#endif /* VECTOR_FILTTER_H_ */
