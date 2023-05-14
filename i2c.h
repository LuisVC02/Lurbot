/**
	\file
	\brief

	\author Luis Ruben Padilla Ortiz & Luis Roberto Vega Campos
			ruben.padilla@iteso.mx		luis.vega@iteso.mx
	\date	14/03/2023
	\todo


 */
#ifndef i2c_H_
#define i2c_H_

#include "stdint.h"
#include "fsl_clock.h"
#include "fsl_port.h"
#include "fsl_i2c.h"

typedef enum
{
	I2C_0, 	/*!< Definition to select I2C0 */
	I2C_1, 	/*!< Definition to select I2C1 */
	I2C_2, 	/*!< Definition to select I2C2 */
} i2c_name_t;



typedef struct i2c_port
{
	clock_ip_name_t clock_port;
	PORT_Type *base;
	uint8_t SLC;
	uint8_t SDA;
	port_mux_t mux;
}i2c_port_t;

void port_initialize_i2c(i2c_name_t n_i2c);
void I2C_init(i2c_name_t n_i2c, uint32_t baud_rate);

status_t read_blocking(i2c_name_t n_i2c, i2c_master_transfer_t *transfer);

#endif /* i2c_H_ */
