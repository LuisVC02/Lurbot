//
//#include <stdio.h>
//#include <stdbool.h>
//#include "pixy2LineTracking.h"
//#include "telemetry.h"
//#include "pit.h"
//#include "nvic.h"
//#include "vector_filtter.h"
//
//#define MAX_VECS 10
//
//void getValues();
//
//void main()
//{
//	Init_Pixy2();
//	telemetry_init();
//
//	PIT_Initialization(
//			PIT,
//			false,
//			T0,
//			20000, 						//Time to get 50 Hz
//			u_seconds,
//			true
//			);
//	PIT_callback_init(T0, getValues);
//
//	NVIC_enable_interrupt_and_priotity(PIT_CH0_IRQ, PRIORITY_4);
//	NVIC_enable_interrupt_and_priotity(UART4_IRQ, PRIORITY_4);
//	NVIC_global_enable_interrupts;
//
//	while(1)
//	{
//
//	}
//
//
//}
//
//void getValues()
//{
//	uint8_t i = 0;
////	uint8_t *			uint8Ptr			= NULL;
//	featureTypeBuff_t * vectorFeaturePrt 	= NULL;
//	vector_t *			vectorPtr 		 	= NULL;
//	vector_t			vectorSnd[MAX_VECS] = {0};
//	direction_t			dirrBuff[MAX_VECS]	= {0};
//	vectorFeaturePrt = getMainFeatures_LinePixy2(lineVector);
//	vectorPtr = (vector_t*)&vectorFeaturePrt->featureData;
//	for(i = 0; i < vectorFeaturePrt->featureLen / 6; i++)
//	{
//		vectorSnd[i] = vectorPtr[i];
//	}
//	getDirectionVecs(vectorSnd, vectorFeaturePrt->featureLen/6, dirrBuff);
//	telemetry_send_unblocking(vectorFeaturePrt->featureLen, (uint8_t *)vectorSnd);
//	i = i;
//}


/*
 * date   : 12/June/2023
 * author : Luis Roberto Vega Campos &
 * 			Luis Rubén Padilla Ortiz &
 * 			Arlette Gabriela Uribe Ventura
 * brief  :
 */

#include <stdint.h>
#include <stdbool.h>
#include "fsl_clock.h"
#include "fsl_port.h"
#include "gpio.h"
#include "nvic.h"
#include "control_system.h"
#include "traction.h"
#include "direction.h"
#include "rc_control.h"
#include "speed_sensor.h"
#include "telemetry.h"
#include "pit.h"
#include "uart.h"
#include "pixy2LineTracking.h"
#include "pixy2TC.h"
#include "pixy2.h"

#define AUTO_MODE_CONTROL_VALUE   2000
#define MANUAL_MODE_CONTROL_VALUE 1000

#define SLOW_MODE_CONTROL_VALUE   1000
#define NORMAL_MODE_CONTROL_VALUE 1500
#define FAST_MODE_CONTROL_VALUE   2000

#define MAX_VECS 10
#define MAX_BUFF 100

typedef struct _valuesToSend
{
	uint8_t  start_value;
	uint16_t ftm_counter;
	int8_t   slope;
	uint8_t  state;
}valuesToSend_t;

typedef enum
{
	noAbleToDeterminate,
	straightforward,
	turnRight,
	turnLeft,
}states_t;

void automatic_mode();
void manual_mode();
void discrete_system();

volatile static channel_controller_t g_control_values;
volatile static uint8_t              g_speed_divisor   = 1;
volatile static bool                 g_automatic       = false;



int main()
{
	// CLOCK initialization ----------------------------------------------------------
	CLOCK_SetSimSafeDivs();
	// -------------------------------------------------------------------------------

	// Traction initialization -------------------------------------------------------
	init_traction(discrete_system);
	// -------------------------------------------------------------------------------

	// Direction initialization ------------------------------------------------------
	init_direction(discrete_system);
	// -------------------------------------------------------------------------------

	// RC initialization -------------------------------------------------------------
	RC_init();
	// -------------------------------------------------------------------------------

	// Speed sensor initialization ---------------------------------------------------
	init_speed_sensor();
	// -------------------------------------------------------------------------------

	// TELEMETRY initialization ------------------------------------------------------
	telemetry_init();
	// -------------------------------------------------------------------------------

	// Pixy2 initialization ------------------------------------------------------
	Init_Pixy2();
	setLamp(true, true);
	// --------------

	// NVIC intialization ------------------------------------------------------------
	NVIC_enable_interrupt_and_priotity(DMA_CH0_IRQ, PRIORITY_3);
	NVIC_enable_interrupt_and_priotity(PIT_CH0_IRQ, PRIORITY_4);
	NVIC_enable_interrupt_and_priotity(FTM0_IRQ, PRIORITY_2);
	NVIC_enable_interrupt_and_priotity(FTM2_IRQ, PRIORITY_1);
	NVIC_enable_interrupt_and_priotity(UART4_IRQ, PRIORITY_5);
	NVIC_global_enable_interrupts;
	// -------------------------------------------------------------------------------


	while(1)
	{
		g_control_values = get_contol_values();

		uint16_t speed_mode = g_control_values.sw2_H << 8;
		speed_mode |= g_control_values.sw2_L;

		uint16_t mode = g_control_values.sw1_H << 8;
		mode |= g_control_values.sw1_L;

		if(true == g_control_values.good_comunication)
		{
			// States machine for auto and manual mode --------------
			if(MANUAL_MODE_CONTROL_VALUE == mode)
			{
				manual_mode();
			}
			else
			{
				automatic_mode();
			}
			// ------------------------------------------------------

			// Selector of speed mode -------------------------------
			if(SLOW_MODE_CONTROL_VALUE == speed_mode)
			{
				g_speed_divisor = 3;
			}
			else if(NORMAL_MODE_CONTROL_VALUE == speed_mode)
			{
				g_speed_divisor = 2;
			}
			else
			{
				g_speed_divisor = 1;
			}
			// ------------------------------------------------------
		}
		else
		{
			set_speed(0);
			set_angle(0);
		}

	}
	return 0;
}

void automatic_mode()
{
	g_automatic = true;
}

void manual_mode()
{
	g_automatic = false;
	int16_t speed = g_control_values.motor_H << 8;
	speed |= g_control_values.motor_L;
	speed -= 1500;
	speed /= g_speed_divisor;
	speed += 1500;

	uint16_t angle = g_control_values.direction_H << 8;
	angle |= g_control_values.direction_L;

	angle = 3000 - angle;

	set_pwm_traction_time(speed);
	set_pwm_direction_time(angle);
}

void discrete_system()
{
	static volatile valuesToSend_t	bufferSnd           = {0};
	static vector_t					vectorCopy[MAX_VECS] = {0};
	featureTypeBuff_t * 			featurePrt 			= NULL;
	vector_t *						vectorPtr 		 	= NULL;
	uint8_t 						vecLen				= 0;
	int8_t 							dirr				= 0;
	uint8_t 						i					= 0;
	bool							vectorFlag 			= false;

	/* Get vectors*/
	featurePrt 	= getMainFeatures_LinePixy2(lineVector);
	if(NULL != featurePrt)
	{
		vecLen 		= featurePrt->featureLen/6;
		vectorPtr 	= (vector_t*)&featurePrt->featureData;
		for(i = 0; i < vecLen; i++)
		{
			vectorCopy[i] = vectorPtr[i];
		}
	}

	/* Calculate direction of vectors*/
	vectorFlag = vectorFilter(vectorCopy, vecLen, &dirr);

	/* Fill our Buffer */
	if(true == vectorFlag)
	{
		/* Chose where to go, based on slope */
		bufferSnd.slope = dirr;
	}
	bufferSnd.start_value = 236;
	bufferSnd.ftm_counter = get_speed().ftm_count;
	telemetry_send_unblocking(2, (uint8_t *)&(bufferSnd.ftm_counter));

//	static volatile valuesToSend_t* bufferSndPtr 		= bufferSnd;
//	static speed_values_to_send_t	speed_values    	= {0};
//	static vector_t					vectorSnd[MAX_VECS] = {0};
//	static direction_t				dirrBuff[MAX_VECS]	= {0};
//
//	featureTypeBuff_t * 			featurePrt 			= NULL;
//	vector_t *						vectorPtr 		 	= NULL;
//	uint8_t *						uint8Ptr			= NULL;
//	uint8_t *						uint8Ptr2			= NULL;
//	uint8_t 						i 					= 0;
//	uint8_t							cnt					= 0;
//	uint8_t 						vecLen				= 0;
//
//	/* Get speed values*/
//	speed_values 	= get_speed();
//
//	/* Get vectors*/
//	featurePrt 	= getMainFeatures_LinePixy2(lineVector);
//	if(NULL != featurePrt)
//	{
//		vecLen 		= featurePrt->featureLen/6;
//		vectorPtr 	= (vector_t*)&featurePrt->featureData;
//		for(i = 0; i < vecLen; i++)
//		{
//			vectorSnd[i] = vectorPtr[i];
//		}
//		/* Calculate direction of vectors*/
//		getDirectionVecs(vectorSnd, vecLen, dirrBuff);
//	}
//
//	/* Fill our Buffer */
//	bufferSndPtr->speed_values 		= speed_values;
//	bufferSndPtr->direction_angle 	= 10;
//	bufferSndPtr->actual_angle 		= 9;
//	bufferSndPtr->vectorLen 		= vecLen*5;
//
//	uint8Ptr = (uint8_t*)&bufferSndPtr->vectorLen;
//	uint8Ptr ++;
//	uint8Ptr2 = (uint8_t*)vectorPtr;
//	for(i = 0; i < vecLen; i++)
//	{
//		for(cnt = 0; cnt < 4; cnt++)
//		{
//			uint8Ptr[i*4 + cnt + i] = uint8Ptr2[i*6 + cnt];
//		}
//		uint8Ptr[i*4 + cnt + i + 1] = dirrBuff[i].slope;
//
//	}
//
//
//
//	/* Send Buffer */
//	telemetry_send_unblocking(9 + featurePrt->featureLen, (uint8_t *)bufferSnd);

	/* Break point */

	i = i;

	if(true == g_automatic)
	{
		control_traction_system(3.7);
	}
}
