/*
 * date   : 12/June/2023
 * author : Luis Roberto Vega Campos &
 * 			Luis Rubén Padilla Ortiz &
 * 			Arlette Gabriela Uribe Ventura
 * brief  :
 */

#include "speed_sensor.h"

#define FTM_FRECUENCY       437500.0
#define RELATION_CALCULATED 20417 // Divide this with input capture, results in speed in m/s
#define SPEED_SENSOR_DIG_FILT_WITH 0x1F

volatile static speed_sensor_values_t g_values = {0};

void init_speed_sensor()
{
	const port_pin_config_t in_config =
	{
			kPORT_PullUp,                                            /* Internal pull-up resistor is enabled */
			kPORT_FastSlewRate,                                      /* Fast slew rate is configured */
			kPORT_PassiveFilterDisable,                              /* Passive filter is disabled */
			kPORT_OpenDrainDisable,                                  /* Open drain is disabled */
			kPORT_HighDriveStrength,                                 /* High drive strength is configured */
			SPEED_SENSOR_ALTERNATIVE,                                         /* Pin is configured as PTA4 */
			kPORT_UnlockRegister                                     /* Pin Control Register fields [15:0] are not locked */
	};
//	const port_digital_filter_config_t digitalFilterCnf =
//	{
//			SPEED_SENSOR_DIG_FILT_WITH,
//			kPORT_BusClock
//	};

	CLOCK_EnableClock(SPEED_SENSOR_PORT_CLOCK);

//	PORT_SetDigitalFilterConfig(SPEED_SENSOR_PORT, &digitalFilterCnf);
//	PORT_EnablePinsDigitalFilter(SPEED_SENSOR_PORT, SPEED_SENSOR_PIN, true);

	PORT_SetPinConfig(SPEED_SENSOR_PORT, SPEED_SENSOR_PIN, &in_config);

	init_input_capture(SPEED_SENSOR_FTM, SPEED_SENSOR_CHANNEL, SPEED_SENSOR_PRESCALER);
	config_input_capture(SPEED_SENSOR_FTM, SPEED_SENSOR_CHANNEL, kFTM_FallingEdge, 0, 100000);
}


speed_sensor_values_t get_speed_sensor()
{
	uint16_t input_capture_value = get_input_capture_value(SPEED_SENSOR_FTM, SPEED_SENSOR_CHANNEL);
	g_values.counter = input_capture_value;
	if(10 < input_capture_value)
	{
		// Revolutions per second of differential ----------------------------------------------
		g_values.rpm_diff = FTM_FRECUENCY/g_values.counter;
		// Revolutions per second of whole -----------------------------------------------------
		//g_values.rpm_whole = g_values.rpm_diff*0.3333333;
		// Speed in meters per second ----------------------------------------------------------
		g_values.speed_m_s = g_values.rpm_whole*0.0733037;
		// Speed in kilometers per hour --------------------------------------------------------
		//g_values.speed_km_hr = g_values.speed_m_s*0.2777;
	}
	else
	{
		// Revolutions per second of differential ----------------------------------------------
		g_values.rpm_diff = 0;
		// Revolutions per second of whole -----------------------------------------------------
		//g_values.rpm_whole = 0;
		// Speed in meters per second ----------------------------------------------------------
		g_values.speed_m_s = 0;
		// Speed in kilometers per hour --------------------------------------------------------
		//g_values.speed_km_hr = 0;
	}
	return g_values;
}
