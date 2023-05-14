/*
 * brief   : This module has line sensor functions
 * date    : 11/May/2023
 * authors :
 * 			Luis Roberto Vega Campos &
 * 			Luis Ruben Padilla Ortiz
 */

#include "line_sensor.h"

volatile static uint8_t g_line_sensor_value = 0;

volatile static i2c_master_transfer_t g_line_sensor_transfer =
{
		kI2C_TransferDefaultFlag,
		0u,
		kI2C_Read,
		0,
		0,
		NULL,
		1
};

void init_sensor()
{
	I2C_init(I2C_0, 500000);
}

uint8_t get_sensorValues()
{
	g_line_sensor_transfer.data         = (uint8_t*)&g_line_sensor_value;
	g_line_sensor_transfer.dataSize     = 1;
	g_line_sensor_transfer.slaveAddress = 0;
	g_line_sensor_transfer.direction    = kI2C_Read;
	g_line_sensor_transfer.flags        = kI2C_TransferDefaultFlag;
	if(kStatus_Success != read_blocking(I2C_0, (i2c_master_transfer_t*)&g_line_sensor_transfer))
	{
		g_line_sensor_value = 0;
	}
	return g_line_sensor_value;
}
