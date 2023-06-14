/*
 * brief : This module has functions for spi control
 * author: Luis Robtero Vega Campos & Luis Ruben Padilla Oritz
 * date  : 28/March/2023
 */

#include "spi.h"



typedef struct
{
	SPI_Type *spi;
	clock_name_t spi_clock;
	clock_ip_name_t port_clock;
}spi_values_t;

spi_values_t ALL_SPI[] =
{
		{SPI0, SPI0_CLK, PORTD_CLK},
		{SPI1, SPI1_CLK, PORTD_CLK},
		{SPI2, SPI2_CLK, PORTD_CLK}
};


volatile static uint8_t g_tx_data[N_SPI] = {0};
volatile static uint8_t g_rx_data[N_SPI] = {0};
volatile static dspi_half_duplex_transfer_t g_transfer[N_SPI] = {0};


void SPI_init(spi_modules_t module, PORT_Type *port, clock_ip_name_t port_clock, dspi_master_config_t *spi_config, uint32_t first_pin)
{
	uint32_t clock_frec = CLOCK_GetFreq(ALL_SPI[module].spi_clock);
	CLOCK_EnableClock(port_clock);
    port_pin_config_t port_config = {
                                   kPORT_PullDisable,  // No pull up
                                   kPORT_FastSlewRate,
                                   kPORT_PassiveFilterDisable,
                                   kPORT_OpenDrainDisable,
                                   kPORT_HighDriveStrength,
								   kPORT_MuxAlt2,
                                   kPORT_UnlockRegister};
	PORT_SetPinConfig(port, first_pin, &port_config);
	PORT_SetPinConfig(port, first_pin+1, &port_config);
	PORT_SetPinConfig(port, first_pin+2, &port_config);
	PORT_SetPinConfig(port, first_pin+3, &port_config);
	DSPI_MasterInit(ALL_SPI[module].spi, spi_config, clock_frec);
}

void SPI_port_and_pin_enable(PORT_Type *port, clock_ip_name_t port_clock, uint32_t pin)
{
	CLOCK_EnableClock(port_clock);
    const port_pin_config_t port_config = {
                                   kPORT_PullDisable,  // No pull up
                                   kPORT_FastSlewRate,
                                   kPORT_PassiveFilterDisable,
                                   kPORT_OpenDrainDisable,
                                   kPORT_HighDriveStrength,
								   kPORT_MuxAlt2,
                                   kPORT_UnlockRegister};
	PORT_SetPinConfig(port, pin, &port_config);
}



status_t SPI_transfer(spi_modules_t module, data_transfer_t transfer_data)
{
	g_transfer[module].txData = transfer_data.tx_Data;
	g_transfer[module].rxData = transfer_data.rx_Data;
	g_transfer[module].txDataSize = transfer_data.n_tx_Data;
	g_transfer[module].rxDataSize = transfer_data.n_rx_Data;
	g_transfer[module].configFlags = transfer_data.chip_select| transfer_data.chip_select_mode| transfer_data.ctar;
	g_transfer[module].isPcsAssertInTransfer = true;
	g_transfer[module].isTransmitFirst = true;

	return DSPI_MasterHalfDuplexTransferBlocking(ALL_SPI[module].spi, (dspi_half_duplex_transfer_t*)(g_transfer));
}
