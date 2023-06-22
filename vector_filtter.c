/*
 * date   : 12/June/2023
 * author : Luis Roberto Vega Campos &
 * 			Luis Rubén Padilla Ortiz &
 * 			Arlette Gabriela Uribe Ventura
 * brief  :
 */

#include "vector_filtter.h"




void findIndexUnderTheshold(vector_t* vectorBuff, uint8_t vecLen )
{



}

/*!
 * brief Calculate up screen side.
 *
 * note index of vector is modified to store this info.
 *
 * param vecBuff 	I2C peripheral base address.
 * param vecLen 	Pointer to the transfer structure.
 */
void validateVectors(vector_t* vecBuff, uint8_t vecLen)
{
	uint8_t i = 0;
	for(i = 0; i < vecLen; i++)
	{
		/* Validate vector begins at the bottom of the screen */
		if(vecBuff[i].m_y0 < vecBuff[i].m_y1)
		{
			swapVector(&vecBuff[i]);
		}

		if(vecBuff[i].m_x0)
		{

		}
	}
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
