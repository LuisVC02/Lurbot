/*
 * brief : This module has functions for spi control
 * author: Luis Robtero Vega Campos & Luis Ruben Padilla Oritz
 * date  : 28/March/2023
 */

#ifndef SPI_H_
#define SPI_H_

#include <stdint.h>
#include "fsl_dspi.h"
#include "fsl_clock.h"
#include "fsl_port.h"

#define N_SPI 3

#define SPI0_CLK DSPI0_CLK_SRC
#define SPI1_CLK DSPI1_CLK_SRC
#define SPI2_CLK DSPI2_CLK_SRC

#define PORTA_CLK kCLOCK_PortA
#define PORTB_CLK kCLOCK_PortB
#define PORTC_CLK kCLOCK_PortC
#define PORTD_CLK kCLOCK_PortD
#define PORTE_CLK kCLOCK_PortE


#define TIME_ADJUST             1000000000u

typedef enum
{
	spi0 = 0,
	spi1 = 1,
	spi2 = 2
}spi_modules_t;

typedef enum
{
	ctar0 = kDSPI_MasterCtar0,
	ctar1 = kDSPI_MasterCtar1,
}ctar_t;

typedef enum
{
	cs0 = kDSPI_MasterPcs0,
	cs1 = kDSPI_MasterPcs1,
	cs2 = kDSPI_MasterPcs2,
	cs3 = kDSPI_MasterPcs3,
	cs4 = kDSPI_MasterPcs4,
	cs5 = kDSPI_MasterPcs5,
}cs_t;

typedef enum
{
	cs_continuos = kDSPI_MasterPcsContinuous,
	cs_after_transfer = kDSPI_MasterActiveAfterTransfer
}cs_mode;

typedef struct
{
	uint8_t * tx_Data;
	uint8_t * rx_Data;
	uint32_t n_tx_Data;
	uint32_t n_rx_Data;
	ctar_t ctar;
	cs_t chip_select;
	cs_mode chip_select_mode;
}data_transfer_t;

void SPI_init(spi_modules_t module, PORT_Type *port, clock_ip_name_t port_clock, dspi_master_config_t *spi_config, uint32_t first_pin);

void SPI_port_and_pin_enable(PORT_Type *port, clock_ip_name_t port_clock, uint32_t pin);

status_t SPI_transfer(spi_modules_t module, data_transfer_t transfer_data);

void SPI_config(spi_modules_t module, dspi_master_config_t *spi_config);


#endif /* SPI_H_ */
