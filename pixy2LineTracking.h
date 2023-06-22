/*
 * This file is for defining the Block struct lineTracking
 *
 *  Created on: 26 abr 2023
 *      Author: otzru
 */

#ifndef PIXY2LINETRACKING_H_
#define PIXY2LINETRACKING_H_

#include "Pixy2.h"

#define LINE_REQUEST_GET_FEATURES                0x30
#define LINE_RESPONSE_GET_FEATURES               0x31
#define LINE_REQUEST_SET_MODE                    0x36
#define LINE_REQUEST_SET_VECTOR                  0x38
#define LINE_REQUEST_SET_NEXT_TURN_ANGLE         0x3a
#define LINE_REQUEST_SET_DEFAULT_TURN_ANGLE      0x3c
#define LINE_REQUEST_REVERSE_VECTOR              0x3e

#define LINE_GET_MAIN_FEATURES                   0x00
#define LINE_GET_ALL_FEATURES                    0x01

#define LINE_MODE_TURN_DELAYED                   0x01
#define LINE_MODE_MANUAL_SELECT_VECTOR           0x02
#define LINE_MODE_WHITE_LINE                     0x80

// features
#define LINE_VECTOR                              0x01
#define LINE_INTERSECTION                        0x02
#define LINE_BARCODE                             0x04
#define LINE_ALL_FEATURES                        (LINE_VECTOR | LINE_INTERSECTION | LINE_BARCODE)

#define LINE_FLAG_INVALID                        0x02
#define LINE_FLAG_INTERSECTION_PRESENT           0x04

#define LINE_MAX_INTERSECTION_LINES              6


typedef enum
{
	lineVector 			= 1u,
	lineIntersection 	= 2u,
	lineBarcode			= 4u,
}featureType_t;

typedef struct _vector
{
	uint8_t m_x0;			/*This variable contains the x location of the tail of the Vector or line.
	 	 	 	 	 	  	  The value ranges between 0 and frameWidth (79)*/
	uint8_t m_y0;			/*This variable contains the y location of the intersection.
	 	 	 	 	 	  	  The value ranges between 0 and frameHeight (52).*/
	uint8_t m_x1;			/*This variable contains the x location of the head (arrow end) of the Vector or line.
	 	 	 	 	 	  	  The value ranges between 0 and frameWidth (79).*/
	uint8_t m_y1;			/*This variable contains the y location of the head (arrow end) of the Vector or line.
	 	 	 	 	 	  	  The value ranges between 0 and frameWidth (52).*/
	uint8_t m_index;		/*This variable contains the tracking index of the Vector or line.*/
	uint8_t m_flags;		/*This variable contains various flags that might be useful.
	 	 	 	 	 	  	  LINE_FLAG_INTERSECTION_PRESENT or LINE_FLAG_INVALID*/
}vector_t;


typedef struct _intersectionLine
{
	uint8_t m_index;		/*This variable contains the tracking index of the line.*/
	uint8_t m_reserved;
	int16_t m_angle;		/*This variable contains the angle in degrees of the line.*/
}intersectionLine_t;

typedef struct _intersection
{
	uint8_t m_x;			/*This variable contains the x location of the intersection. The value ranges between 0 and frameWidth (79).*/
	uint8_t m_y;			/*This variable contains the y location of the intersection. The value ranges between 0 and frameHeight (52).*/

	uint8_t m_n;			/*This variable contains the number of lines (branches) in the intersection and in the m_intLines array.*/
	uint8_t m_reserved;
	intersectionLine_t* m_intLines;	/*This array contains the lines in the intersection */
}intersection_t;

typedef struct _featureTypeBuff
{
	uint8_t featureType;
	uint8_t featureLen;
	uint8_t featureData;
}featureTypeBuff_t;

featureTypeBuff_t* getMainFeatures_LinePixy2(featureType_t featureType);

#endif /* PIXY2LINETRACKING_H_ */
