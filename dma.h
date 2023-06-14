/*
 * brief : This module has dma functions
 * author: Luis Robtero Vega Campos & Luis Ruben Padilla Ortiz
 * date  : 31/March/2023
 */

#ifndef DMA_H_
#define DMA_H_

#include "fsl_edma.h"
#include "fsl_dmamux.h"
#include <stdint.h>
#include <stdbool.h>
#include "MK66F18.h"

/***************************************************************
                       MACRO DEFINITIONS
****************************************************************/
#define TCD_QUEUE_SIZE      2U
/***************************************************************
                     END MACRO DEFINITIONS
****************************************************************/

/***************************************************************
                       TYPE DEFINITIONS
****************************************************************/
typedef enum
{
	disable,
	tsi,
	uart0_rx,
	uart0_tx,
	uart1_rx,
	uart1_tx,
	uart2_rx,
	uart2_tx,
	uart3_rx,
	uart3_tx,
	uart4,
	reserved,
	iis_rx,
	iis_tx,
	spi0_rx,
	spi0_tx,
	spi1_dma,
	spi2_dma,
	iic0,
	iic1_2,
	FTM0_0,
	FTM0_1,
	FTM0_2,
	FTM0_3,
	FTM0_4,
	FTM0_5,
	FTM0_6,
	FTM0_7,
	FTM1_0,
	FTM1_1,
	FTM2_0,
	FTM2_1,
	FTM3_0,
	FTM3_1,
	FTM3_2,
	FTM3_3,
	FTM3_4,
	FTM3_5,
	FTM3_6,
	FTM3_7,
	adc0,
	adc1,
	cmp0,
	cmp1,
	cmp2,
	dac0,
	dac1,
	cmt,
	pdb,
	port_a,
	port_b,
	port_c,
	port_d,
	port_e,
	timer0,
	timer1,
	timer2,
	timer3,
	lpuart0_rx,
	lpuart0_tx,
	dma_mux_0,
	dma_mux_1,
	dma_mux_2,
	dma_mux_3
}dmamux_src_t;
/***************************************************************
                     END TYPE DEFINITIONS
****************************************************************/

void DMA_init(uint32_t channel, edma_callback callback);

void set_dmamux(uint32_t channel, dmamux_src_t source, bool period_trigger);

void enable_channel(uint32_t channel, bool enable);

void set_transfer_config(void *src_Addr,
						 uint32_t src_size,
						 int16_t src_offset_minor,
						 int32_t src_offset_major,
						 void *dest_Addr,
						 uint32_t dest_size,
						 int16_t dest_offset_minor,
						 int32_t dest_offset_major,
						 uint32_t bytesEachRequest,
						 uint32_t transferBytes,
						 bool     enable_auto_stop_major,
						 uint32_t channel);

#endif /* DMA_H_ */
