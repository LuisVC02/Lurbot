/*
 * author: Luis Roberto Vega Campos
 * date  : 10/April/2023
 * brief : This module has a led rgb functions
 */

#ifndef LEDS_H_
#define LEDS_H_

#include "gpio.h"

#define RED_PIN    22u
#define RED_PORT   GPIO_B

#define GREEN_PIN  26u
#define GREEN_PORT GPIO_E

#define AMB_PIN   21u
#define AMB_PORT  GPIO_B

typedef enum
{
	red_led,
	green_led,
	amb_led
}simple_leds_t;

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

void RGB_init();

void RGB_setColor(colors_t color);

#endif /* LEDS_H_ */
