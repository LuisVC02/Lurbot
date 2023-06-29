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
#include "leds.h"

#define AUTO_MODE_CONTROL_VALUE   2000
#define MANUAL_MODE_CONTROL_VALUE 1000

#define SLOW_MODE_CONTROL_VALUE   1000
#define NORMAL_MODE_CONTROL_VALUE 1500
#define FAST_MODE_CONTROL_VALUE   2000

#define SLOPE_GAIN_DIRECTION_1    -4
#define SLOPE_GAIN_DIRECTION_2    -5
#define SLOPE_GAIN_DIRECTION_3    -6
#define MID_SCREEN_SIZE           39
#define SCREEN_SIZE               78
#define FILTTER_ORDER             2

#define MAX_VECS 10
#define MAX_BUFF 100

typedef struct _valuesToSend
{
	uint16_t 	ftm_counter;
	vector_t 	main_vecs[2];
	int16_t  	slope;
	uint8_t  	state;
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

static volatile valuesToSend_t	bufferSnd           = {0};
static volatile valuesToSend_t	bufferSnd2           =
{
		1,
		{{1, 2, 3, 4, 5, 6}, {7, 8, 9, 10, 11, 12}},
		13,
		14
};

const float low_pass[3][(FILTTER_ORDER+1)*2] =
{
{1.000000000000000, 1.142980502539901, -0.412801598096189, 0.067455273889072, 0.134910547778144, 0.067455273889072}, //5Hz
{1.000000000000000, 1.647459981076977, -0.700896781188403 ,0.013359200027857, 0.026718400055713, 0.013359200027856}, //2Hz
{1.000000000000000, 0.369527377351241, -0.195815712655833 ,0.206572083826148, 0.413144167652296, 0.206572083826148}  //10Hz
};
volatile static channel_controller_t g_control_values;
volatile static uint8_t              g_speed_divisor   = 1;
volatile static bool                 g_automatic       = false;
volatile static float                g_max_speed       = 0;
volatile static uint8_t              g_divisor         = 0;


volatile static float                g_last_in_slopes[FILTTER_ORDER+1]  = {0};
volatile static float                g_last_out_slopes[FILTTER_ORDER+1] = {0};
volatile static int8_t               g_index_filtter                    =  0;
volatile static int8_t               g_gain_direction 					=  0;
volatile static uint8_t              g_filtter                          =  0;


int main()
{
	// CLOCK initialization ----------------------------------------------------------
	CLOCK_SetSimSafeDivs();
	// -------------------------------------------------------------------------------

	// LEDs initialization ------------------------------------------------------
	LEDS_init(false);
	RGB_setColor(red);
	gpio_pin_conf_t config =
	{
			GPIO_D,
			4,
			true,
			false,
			InterrupDisable
	};
	GPIO_init(config);
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
	while(!getVersion());
	setLamp(true, true);
	// -------------------------------------------------------------------------------

	// NVIC intialization ------------------------------------------------------------
	NVIC_enable_interrupt_and_priotity(DMA_CH0_IRQ, PRIORITY_3);
	NVIC_enable_interrupt_and_priotity(PIT_CH0_IRQ, PRIORITY_4);
	NVIC_enable_interrupt_and_priotity(FTM0_IRQ, PRIORITY_2);
	NVIC_enable_interrupt_and_priotity(FTM2_IRQ, PRIORITY_1);
	NVIC_enable_interrupt_and_priotity(UART4_IRQ, PRIORITY_5);
	NVIC_global_enable_interrupts;
	// -------------------------------------------------------------------------------

	RGB_setColor(green);
	while(1)
	{
		g_control_values = get_contol_values();

		uint16_t speed_mode = g_control_values.sw2_H << 8;
		uint16_t gain_direction = g_control_values.sw3_H<<8;
		uint16_t filter_select  = g_control_values.sw4_H<<8;
		speed_mode |= g_control_values.sw2_L;
		gain_direction |= g_control_values.sw3_L;
		filter_select  |= g_control_values.sw4_L;

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
				g_max_speed     = 3;
				g_divisor       = 30;
				g_gain_direction = SLOPE_GAIN_DIRECTION_1;
			}
			else if(NORMAL_MODE_CONTROL_VALUE == speed_mode)
			{
				g_speed_divisor = 2;
				g_max_speed     = 3.5;
				g_divisor       = 20;
				g_gain_direction = SLOPE_GAIN_DIRECTION_2;
			}
			else
			{
				g_speed_divisor = 1;
				g_max_speed     = 4;
				g_divisor       = 10;
				g_gain_direction = SLOPE_GAIN_DIRECTION_3;
			}
			// ------------------------------------------------------
			// Select gain of direction -----------------------------
			if(SLOW_MODE_CONTROL_VALUE == gain_direction)
			{
				g_filtter = 0;
			}
			else if(NORMAL_MODE_CONTROL_VALUE == gain_direction)
			{
				g_filtter = 1;
			}
			else
			{
				g_filtter = 2;
			}
			// ------------------------------------------------------
			if(SLOW_MODE_CONTROL_VALUE == filter_select)
			{

				GPIO_clear_values(GPIO_D, 1<<4);
			}
			else
			{
				GPIO_set_values(GPIO_D, 1<<4);
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
	static vector_t					vectorCopy[MAX_VECS]= {0};
	static int16_t 					initial_slope 		=  0;
	static float 					aux_slope 		    =  0;
	int8_t                          x_prom              =  0;
	featureTypeBuff_t * 			featurePrt 			= NULL;
	vector_t *						vectorPtr 		 	= NULL;
	uint8_t 						vecLen				= 0;
	uint8_t 						i					= 0;
	uint8_t							validVector			= false;


	/* Indicate State*/
	RGB_setColor(red);
	initial_slope = 0;
/*	bufferSnd.terminator[0] = "C";
	bufferSnd.terminator[0] = "R";*/
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
		RGB_setColor(blue);
		/* Calculate direction of vectors*/
		validVector = vectorFilter(vectorCopy, vecLen, &initial_slope, &x_prom);

		/* Fill our Buffer */
		if(0 != validVector)
		{
			/* Chose where to go, based on slope */
			aux_slope = initial_slope;
		}
		else
		{
			initial_slope = 0;
			x_prom = MID_SCREEN_SIZE;
			validVector = 2;
		}
		bufferSnd.state = 10;
		bufferSnd.slope = (int16_t)initial_slope;
	}
	// Flitter section ------------------------------------------------------------------
	uint8_t index  = 0;
	uint8_t valid_index = 0;
	uint8_t index_filtter = 0;
	aux_slope     = 0;
	g_last_in_slopes[g_index_filtter] = initial_slope;
	g_last_out_slopes[g_index_filtter] = 0;

	for(index = 0, index_filtter = FILTTER_ORDER+1; index <= FILTTER_ORDER; index ++, index_filtter ++)
	{
		valid_index = g_index_filtter + index;
		// Validate index ---------------------------
		valid_index = (valid_index > FILTTER_ORDER)? (valid_index-FILTTER_ORDER-1):valid_index;
		// ------------------------------------------
		if(0 == index)
		{
			g_last_out_slopes[g_index_filtter] = low_pass[g_filtter][index_filtter]*g_last_in_slopes[g_index_filtter];
		}
		else
		{
			g_last_out_slopes[g_index_filtter] += low_pass[g_filtter][index]*g_last_out_slopes[valid_index];
			g_last_out_slopes[g_index_filtter] += low_pass[g_filtter][index_filtter]*g_last_in_slopes[index];
		}
	}
	aux_slope = (int16_t)g_last_out_slopes[g_index_filtter];
	if(0 == g_index_filtter)
	{
		g_index_filtter = FILTTER_ORDER;
	}
	else
	{
		g_index_filtter --;
	}
	// ----------------------------------------------------------------------------------
	bufferSnd.ftm_counter 	= (uint16_t)(get_speed().speed_sensor_ms*100);
	bufferSnd.slope = (int16_t)aux_slope;

 	telemetry_send_unblocking(2u, (uint8_t*)&bufferSnd.slope);

	RGB_setColor(green);

	initial_slope = aux_slope;
	/* Break point */
	i = 0;
	if(true == g_automatic)
	{
		initial_slope     /= 100;
		float less      = 1.0;
		float   new_speed = 0;
		int16_t new_angle = 0;

		if(1 < validVector)
		{
			x_prom            = MID_SCREEN_SIZE - x_prom;
		}
		else
		{
			x_prom            = SCREEN_SIZE - x_prom;
			x_prom            = (MID_SCREEN_SIZE <= x_prom)?   (-SCREEN_SIZE)+x_prom : x_prom;
		}
		//x_prom *= 2;
		if(initial_slope >= 4)
		{
			less -=   ((float)initial_slope/g_divisor);
			new_speed = (float)(g_max_speed)*less;
			new_angle = (initial_slope*g_gain_direction);
		}
		else if(initial_slope <= -4)
		{
			less -=   (-(float)initial_slope/g_divisor);
			new_speed = (float)(g_max_speed)*less;
			new_angle = (initial_slope*g_gain_direction);
		}
		else
		{
			new_speed = g_max_speed;
			//new_angle = 0;
			new_angle = x_prom;
		}
		control_traction_system(new_speed);
		set_angle(new_angle);
	}

}
