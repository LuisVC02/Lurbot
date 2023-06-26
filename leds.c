/*
 * author: Luis Roberto Vega Campos
 * date  : 10/April/2023
 * brief : This module has a led rgb functions
 *
 * todo Implemetar PWM en led RGB
 *
 */

#include <leds.h>

static volatile bool rgbPwmEn = false;

const uint8_t ledsPortsPins[LEDS_AMOUNT][2] =
{
		{GREEN_PORT,	GREEN_PIN},
		{AMB_PORT, 		AMB_PIN},
		{R_RGB_PORT,	R_RGB_PIN_PWM},
		{G_RGB_PORT,	G_RGB_PIN_PWM},
		{B_RGB_PORT,	B_RGB_PIN_PWM}
};

void LEDS_init(bool rgbPwmEnable)
{
	gpio_pin_conf_t led_config =
	{
			GREEN_PORT,
			GREEN_PIN,
			true,
			false,
			InterrupDisable
	};
	ftm_config_t configFtmRBG = {0};
	ftm_chnl_pwm_signal_param_t rbgParm[3] =
	{
			{R_RGB_CHANNEL_PWM, FTM_PWM_ON_LEVEL, 0, 0, false, false},
			{R_RGB_CHANNEL_PWM, FTM_PWM_ON_LEVEL, 0, 0, false, false},
			{R_RGB_CHANNEL_PWM, FTM_PWM_ON_LEVEL, 0, 0, false, false}
	};
	GPIO_init(led_config);

	led_config.gpio = AMB_PORT;
	led_config.pin = AMB_PIN;
	GPIO_init(led_config);

	if(true == rgbPwmEnable)
	{
		rgbPwmEn = rgbPwmEnable;
		CLOCK_EnableClock(R_RGB_CLK_PWM);
		CLOCK_EnableClock(G_RGB_CLK_PWM);
		CLOCK_EnableClock(B_RGB_CLK_PWM);
		PORT_SetPinMux(R_RGB_PORT_PWM, R_RGB_PIN_PWM, R_RGB_ALT_PWM);
		PORT_SetPinMux(G_RGB_PORT_PWM, G_RGB_PIN_PWM, G_RGB_ALT_PWM);
		PORT_SetPinMux(B_RGB_PORT_PWM, B_RGB_PIN_PWM, B_RGB_ALT_PWM);

		FTM_GetDefaultConfig(&configFtmRBG);
		/* Calculate the clock division based on the PWM frequency to be obtained */
		configFtmRBG.prescale = FTM_CalculateCounterClkDiv(PWM_TIMER_RGB, PWM_FREQUENCY, FTM_SOURCE_CLOCK);
		FTM_Init(PWM_TIMER_RGB, &configFtmRBG);

		FTM_SetupPwm(PWM_TIMER_RGB, rbgParm, 3U, kFTM_EdgeAlignedPwm, PWM_FREQUENCY, FTM_SOURCE_CLOCK);
		FTM_StartTimer(FTM3, kFTM_SystemClock);
	}
	else
	{
		led_config.gpio = R_RGB_PORT;
		led_config.pin 	= R_RGB_PIN;
		GPIO_init(led_config);
		led_config.gpio = G_RGB_PORT;
		led_config.pin 	= G_RGB_PIN;
		GPIO_init(led_config);
		led_config.gpio = B_RGB_PORT;
		led_config.pin 	= B_RGB_PIN;
		GPIO_init(led_config);
	}

	turn_off_led(green_led);
	turn_off_led(amb_led);
	RGB_setColor(off);
}

void turn_on_led(leds_t led)
{
	if(led >= red_rgb_led)
	{
		if(false == rgbPwmEn)
		{
			GPIO_set_values(ledsPortsPins[led][0], 1 << ledsPortsPins[led][1]);
		}
	}
	else
	{
		GPIO_clear_values(ledsPortsPins[led][0], 1 << ledsPortsPins[led][1]);
	}
}
void turn_off_led(leds_t led)
{
	if(led >= red_rgb_led)
	{
		if(false == rgbPwmEn)
		{
			GPIO_clear_values(ledsPortsPins[led][0], 1 << ledsPortsPins[led][1]);
		}
	}
	else
	{
		GPIO_set_values(ledsPortsPins[led][0], 1 << ledsPortsPins[led][1]);
	}
}
void toggle_led(leds_t led)
{
	if(led >= red_rgb_led)
	{
		if(false == rgbPwmEn)
		{
			GPIO_toggle_values(ledsPortsPins[led][0], 1 << ledsPortsPins[led][1]);
		}
	}
	else
	{
		GPIO_toggle_values(ledsPortsPins[led][0], 1 << ledsPortsPins[led][1]);
	}
}

void RGB_setColor(colors_t color)
{
	if(false == rgbPwmEn)
	{
		switch(color)
		{
		case red:
			GPIO_set_values(R_RGB_PORT, 1<<R_RGB_PIN);
			GPIO_clear_values(B_RGB_PORT, 1<<B_RGB_PIN);
			GPIO_clear_values(G_RGB_PORT, 1<<G_RGB_PIN);
			break;

		case blue:
			GPIO_clear_values(R_RGB_PORT, 1<<R_RGB_PIN);
			GPIO_set_values(B_RGB_PORT, 1<<B_RGB_PIN);
			GPIO_clear_values(G_RGB_PORT, 1<<G_RGB_PIN);
			break;

		case green:
			GPIO_clear_values(R_RGB_PORT, 1<<R_RGB_PIN);
			GPIO_clear_values(B_RGB_PORT, 1<<B_RGB_PIN);
			GPIO_set_values(G_RGB_PORT, 1<<G_RGB_PIN);
			break;

		case purpure:
			GPIO_set_values(R_RGB_PORT, 1<<R_RGB_PIN);
			GPIO_set_values(B_RGB_PORT, 1<<B_RGB_PIN);
			GPIO_clear_values(G_RGB_PORT, 1<<G_RGB_PIN);
			break;

		case yellow:
			GPIO_set_values(R_RGB_PORT, 1<<R_RGB_PIN);
			GPIO_clear_values(B_RGB_PORT, 1<<B_RGB_PIN);
			GPIO_set_values(G_RGB_PORT, 1<<G_RGB_PIN);
			break;

		case white:
			GPIO_set_values(R_RGB_PORT, 1<<R_RGB_PIN);
			GPIO_set_values(B_RGB_PORT, 1<<B_RGB_PIN);
			GPIO_set_values(G_RGB_PORT, 1<<G_RGB_PIN);
			break;

		default:
			GPIO_clear_values(R_RGB_PORT, 1<<R_RGB_PIN);
			GPIO_clear_values(B_RGB_PORT, 1<<B_RGB_PIN);
			GPIO_clear_values(G_RGB_PORT, 1<<G_RGB_PIN);
			break;
		}
	}
}
