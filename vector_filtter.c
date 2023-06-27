/*
 * date   : 12/June/2023
 * author : Luis Roberto Vega Campos &
 * 			Luis Rubén Padilla Ortiz &
 * 			Arlette Gabriela Uribe Ventura
 * brief  :
 */

#include "vector_filtter.h"




//void getDirectionVecs(vector_t * vecsBuff, uint8_t vecsLen, direction_t * directionBuff)
//{
//	uint8_t index = 0;
////	direction_t dir = {0};
//
//	for(index = 0; index < vecsLen; index++)
//	{
//		directionBuff[index] = calculateDirection(vecsBuff[index]);
//	}
//}

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

/*Function that determinate the direction the car should take based on vectors.*/
/*Return Left or Right and slope*/
bool vectorFilter(vector_t* vectorBuff, uint8_t vecLen, int16_t* slopeFound)
{
	bool 		retval						= false;
	uint8_t		i 							= 0;
	int16_t 	validIndexLenLoc			= 0;
	slope_t 	slopenBuff[MAX_VECTS] 		= {0};
	vector_t    vector                      = {0};
	vector_t 	vecTemp 					= {0};


	/*	Look up for valid indexes where the origin is bellow ORIGIN_THRESHOLD  */
	for(i = 0; i < vecLen; i++)
	{
		vector = vectorBuff[i];
		/* Change orientation of vector in y ------------- */
		vector.m_y0 = SCREEN_SIZE_Y - vector.m_y0;
		vector.m_y1 = SCREEN_SIZE_Y - vector.m_y1;
		/* ----------------------------------------------- */
		if(0 == (vector.m_flags & (LINE_FLAG_INVALID | LINE_FLAG_INTERSECTION_PRESENT)))
		{
			if(vector.m_y0 < THRESHOLD || vector.m_y1 < THRESHOLD)
			{
				/*	Swap origin (x0, y0) in case this is higher than final  */
				if( vector.m_y1 < vector.m_y0)
				{
					vecTemp =  vector;
					vector.m_x0 = vecTemp.m_x1;
					vector.m_x1 = vecTemp.m_x0;
					vector.m_y0 = vecTemp.m_y1;
					vector.m_y1 = vecTemp.m_y0;
				}
				/* -------------------------------------------------------- */

				float m = ((float)vector.m_x1 - vector.m_x0)/((float)vector.m_y1 - vector.m_y0);
				if(VALID_SLOPE > m && (-VALID_SLOPE) < m)
				{
					//validIndexBuff[validIndexLenLoc] = i;
					//slopenBuff[validIndexLenLoc] = (int16_t)(m*1000);
					//*slopeFound += slopenBuff[validIndexLenLoc];
					*slopeFound += (int16_t)(m*1000);
					validIndexLenLoc++;
				}

				/*	Looks up side of the screen. Stores on index field. 	*/
				/*if( vector.m_x0 > SCREEN_SIZE_X/2 )
				{
					vectorBuff[i].m_index = RightSide << SCREEN_SIDE_BIT_NUM;
				}
				else
				{
					vectorBuff[i].m_index = LeftSide << SCREEN_SIDE_BIT_NUM;
				}*/


				//			/*	Looks up height of the screen. Stores on index.	*/
				//			if( vectorBuff[i].m_y0 > SCREEN_SIZE_Y/2 )
				//			{
				//				vectorBuff[i].m_index |= bottom << SCREEN_HEIGHT_BIT_NUM;
				//			}
				//			else
				//			{
				//				vectorBuff[i].m_index |= top << SCREEN_HEIGHT_BIT_NUM;
				//			}
			}
		}
	}


	/*	Once we have valid vectors. Make the average.*/
	if(validIndexLenLoc > 0)
	{
		*slopeFound /= validIndexLenLoc;
		retval = true;
	}

	return retval;
}
