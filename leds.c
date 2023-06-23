/*
 * author: Luis Roberto Vega Campos
 * date  : 10/April/2023
 * brief : This module has a led rgb functions
 */

#include <leds.h>

void LEDS_init()
{
	gpio_pin_conf_t led_config =
	{
			GREEN_PORT,
			GREEN_PIN,
			true,
			false,
			InterrupDisable
	};
	GPIO_init(led_config);

	led_config.gpio = AMB_PORT;
	led_config.pin = AMB_PIN;
	GPIO_init(led_config);


	//todo Is left one led
//	led_config.gpio = GREEN_PORT;
//	led_config.pin = GREEN_PIN;
//	GPIO_init(led_config);
	RGB_setColor(off);
}

void turn_on_led(simple_leds_t led, bool on_off)
{

}

void RGB_setColor(simple_leds_t led,colors_t color)
{
	switch(color)
	{
		case red:
			GPIO_clear_values(RED_PORT, 1<<RED_PIN);
			GPIO_set_values(BLUE_PORT, 1<<BLUE_PIN);
			GPIO_set_values(GREEN_PORT, 1<<GREEN_PIN);
		break;

		case blue:
			GPIO_set_values(RED_PORT, 1<<RED_PIN);
			GPIO_clear_values(BLUE_PORT, 1<<BLUE_PIN);
			GPIO_set_values(GREEN_PORT, 1<<GREEN_PIN);
		break;

		case green:
			GPIO_set_values(RED_PORT, 1<<RED_PIN);
			GPIO_set_values(BLUE_PORT, 1<<BLUE_PIN);
			GPIO_clear_values(GREEN_PORT, 1<<GREEN_PIN);
		break;

		case purpure:
			GPIO_clear_values(RED_PORT, 1<<RED_PIN);
			GPIO_clear_values(BLUE_PORT, 1<<BLUE_PIN);
			GPIO_set_values(GREEN_PORT, 1<<GREEN_PIN);
		break;

		case yellow:
			GPIO_clear_values(RED_PORT, 1<<RED_PIN);
			GPIO_set_values(BLUE_PORT, 1<<BLUE_PIN);
			GPIO_clear_values(GREEN_PORT, 1<<GREEN_PIN);
		break;

		case white:
			GPIO_clear_values(RED_PORT, 1<<RED_PIN);
			GPIO_clear_values(BLUE_PORT, 1<<BLUE_PIN);
			GPIO_clear_values(GREEN_PORT, 1<<GREEN_PIN);
		break;

		default:
			GPIO_set_values(RED_PORT, 1<<RED_PIN);
			GPIO_set_values(BLUE_PORT, 1<<BLUE_PIN);
			GPIO_set_values(GREEN_PORT, 1<<GREEN_PIN);
		break;
	}
}
