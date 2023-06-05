// I2C


#include "i2c.h"

#define I2C_MASTER_CLK_SRC          I2C0_CLK_SRC
#define I2C_MASTER_CLK_FREQ         CLOCK_GetFreq(I2C0_CLK_SRC)

typedef I2C_Type *i2c_modules;

volatile static i2c_port_t g_ports_i2c [3]		= {
		/*CLK_PORT	   PORT	  SLC  SDA  Alternative	*/
		{kCLOCK_PortE, PORTE, 24u, 25u, kPORT_MuxAlt5},
		{kCLOCK_PortC, PORTC, 10u, 11u, kPORT_MuxAlt2},
		{kCLOCK_PortA, PORTA, 11u, 12u, kPORT_MuxAlt5},
};

const i2c_modules  g_i2c[3]             = {I2C0, I2C1, I2C2};


/*!
 *  brief Initialize I2CX
 * 	param i2c_n I2C to be initialized
 * 	param baud_rate Baud Rate of I2C
 */
void I2C_init(i2c_name_t i2c_n, uint32_t baud_rate )
{
	port_initialize_i2c(i2c_n);

	i2c_master_config_t masterConfig;
	   /* 	Default baud rate at 100kbps.
		masterConfig->baudRate_Bps = 100000U;
			Default stop hold enable is disabled.
		masterConfig->enableStopHold = false;
			Default glitch filter value is no filter.
		masterConfig->glitchFilterWidth = 0U;
			Enable the I2C peripheral.
		masterConfig->enableMaster = true;*/
    I2C_MasterGetDefaultConfig(&masterConfig);
    masterConfig.baudRate_Bps = baud_rate;

	I2C_MasterInit(g_i2c[i2c_n], &masterConfig, I2C_MASTER_CLK_FREQ);
}

/*!
 *  brief Initialize Port based on I2CX
 * 	param i2c_n I2C number to be set up based on structure i2c_port_t
 */
void port_initialize_i2c(i2c_name_t n_i2c)
{

	/* Port X Clock Gate Control: Clock enabled */
	CLOCK_EnableClock(g_ports_i2c[n_i2c].clock_port);

	/* PORTB (pin 3) is configured as I2C0_SDA */
    const port_pin_config_t pin_config = {/* Internal pull-up resistor is enabled */
                                                    kPORT_PullUp,
                                                    /* Fast slew rate is configured */
                                                    kPORT_FastSlewRate,
                                                    /* Passive filter is disabled */
                                                    kPORT_PassiveFilterDisable,
                                                    /* Open drain is enabled */
                                                    kPORT_OpenDrainEnable,
                                                    /* Low drive strength is configured */
                                                    kPORT_LowDriveStrength,
                                                    /* Pin is configured as I2C0_SCL */
													g_ports_i2c[n_i2c].mux,
                                                    /* Pin Control Register fields [15:0] are not locked */
                                                    kPORT_UnlockRegister};

    PORT_SetPinConfig(g_ports_i2c[n_i2c].base, g_ports_i2c[n_i2c].SDA, &pin_config);
	PORT_SetPinConfig(g_ports_i2c[n_i2c].base, g_ports_i2c[n_i2c].SLC, &pin_config);
}

status_t I2C_ReadBlocking(i2c_name_t n_i2c, i2c_master_transfer_t *transfer)
{
	return I2C_MasterTransferBlocking(g_i2c[n_i2c], transfer);
}

