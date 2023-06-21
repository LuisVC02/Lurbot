/*
 * date   : 12/June/2023
 * author : Luis Roberto Vega Campos &
 * 			Luis Rubén Padilla Ortiz &
 * 			Arlette Gabriela Uribe Ventura
 * brief  :
 */

#include "vector_filtter.h"

static inline direction_t calculateDirection(vector_t vector)
{
	direction_t retDirection = {0};
	retDirection.angle = 0;
	retDirection.slope = (vector.m_y1 - vector.m_y0)/(vector.m_x1 - vector.m_x0);
	return retDirection;
}

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

void getDirectionVecs(vector_t * vecsBuff, uint8_t vecsLen, direction_t * directionBuff)
{
	uint8_t index = 0;
//	direction_t dir = {0};

	for(index = 0; index < vecsLen; index++)
	{
		directionBuff[index] = calculateDirection(vecsBuff[index]);
	}
}

/*
 * If direction is under range returns true on index;
 *
 * Note: Max dirBuffLen 64
 */
void filtSlope(
		slope_t * 	slopeBuff,
		uint8_t 	slopeBuffLen,
		uint64_t * 	maskSlope,
		slope_t 	maxSlope,
		slope_t 	minSlope
		)
{
	uint8_t index = 0;
	for(index = 0; index < slopeBuffLen; index++)
	{
		if(true == validateRangeSlpoe(maxSlope, minSlope, slopeBuff[index]) )
		{
			maskSlope[0] |= (1 << index);
		}
	}
}
