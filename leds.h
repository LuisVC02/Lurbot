/*
 * author: Luis Roberto Vega Campos
 * date  : 10/April/2023
 * brief : This module has a led rgb functions
 */

#ifndef LEDS_H_
#define LEDS_H_

#include "gpio.h"
#include "fsl_port.h"
#include "fsl_ftm.h"

#define LEDS_AMOUNT 5



#define PWM_TIMER_RGB      		FTM3
#define FTM_SOURCE_CLOCK 		CLOCK_GetFreq(kCLOCK_BusClk)
#define FTM_PWM_ON_LEVEL 		kFTM_HighTrue
#define PWM_FREQUENCY 			(24000U)

#define R_RGB_CHANNEL_PWM     	kFTM_Chnl_3
#define R_RGB_PORT_PWM        	PORTD
#define R_RGB_PIN_PWM         	1
#define R_RGB_ALT_PWM         	kPORT_MuxAlt4
#define R_RGB_CLK_PWM         	kCLOCK_PortD

#define G_RGB_CHANNEL_PWM     	kFTM_Chnl_5
#define G_RGB_PORT_PWM        	PORTC
#define G_RGB_PIN_PWM         	9
#define G_RGB_ALT_PWM         	kPORT_MuxAlt3
#define G_RGB_CLK_PWM         	kCLOCK_PortC

#define B_RGB_CHANNEL_PWM     	kFTM_Chnl_4
#define B_RGB_PORT_PWM        	PORTC
#define B_RGB_PIN_PWM         	8
#define B_RGB_ALT_PWM         	kPORT_MuxAlt3
#define B_RGB_CLK_PWM         	kCLOCK_PortC

#define GREEN_PIN  	14u
#define GREEN_PORT 	GPIO_D
#define AMB_PIN   	13u
#define AMB_PORT  	GPIO_D
#define R_RGB_PIN   1u
#define R_RGB_PORT  GPIO_D
#define G_RGB_PIN   9u
#define G_RGB_PORT  GPIO_C
#define B_RGB_PIN   8u
#define B_RGB_PORT  GPIO_C

typedef enum
{
	green_led,
	amb_led,
	red_rgb_led,
	green_rgb_led,
	blue_rgb_led
}leds_t;

typedef enum
{
	red,
	green,
	blue,
	purpure,
	yellow,
	cyan,
	white,
	off
} colors_t;

void LEDS_init(bool rgbPwmEnable);
void turn_on_led(leds_t led);
void turn_off_led(leds_t led);
void toggle_led(leds_t led);
void RGB_setColor(colors_t color);

#endif /* LEDS_H_ */
