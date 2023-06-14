/*
 * brief : This module has dma functions
 * author: Luis Robtero Vega Campos & Luis Ruben Padilla Ortiz
 * date  : 31/March/2023
 */

#include "dma.h"

#define MAX_TRIGGER_CHANEL 3

/***************************************************************
                       GLOBAL VARIABLES
****************************************************************/
AT_QUICKACCESS_SECTION_DATA_ALIGN(edma_tcd_t g_tcdMemoryPoolPtr[TCD_QUEUE_SIZE + 1], sizeof(edma_tcd_t));
volatile static edma_handle_t EDMA_Handle;
/***************************************************************
                     END GLOBAL VARIABLES
****************************************************************/

/*!
 * brief Initialize DMA0 specific channel
 * param channel 	channel number to be initialized
 * param callback 	function to be call when a major loop is done.
 * 					If callback is not going to be initialized send NULL
 */
void DMA_init(uint32_t channel, edma_callback callback)
{

	const edma_config_t edma_config =
	{
			false, // enable continuous link mode
			true,  // enable halt on error
			false, // enable round robin arbitration
			false  // enable debug mode
	};


	EDMA_Init(DMA0, &edma_config);
    if(callback != NULL)
    {
		EDMA_CreateHandle((edma_handle_t*)&EDMA_Handle, DMA0, channel);
		EDMA_SetCallback((edma_handle_t*)&EDMA_Handle, callback, NULL);
	    EDMA_ResetChannel(EDMA_Handle.base, EDMA_Handle.channel);
		EDMA_TcdEnableInterrupts(&g_tcdMemoryPoolPtr[channel], kEDMA_MajorInterruptEnable);
		EDMA_InstallTCD(DMA0, channel, &g_tcdMemoryPoolPtr[channel]);
		EDMA_EnableChannelRequest(DMA0, channel);
    }
}


/*!
 * brief Sets up transfer configuration on specific channel
 * param src_Addr eDMA transfer source address.
 * param src_size eDMA transfer source address width(bytes).
 * param src_offset_minor source address offset minor loop.
 * param src_offset_major source address offset major.
 * param dest_Addr eDMA transfer destination address.
 * param dest_size eDMA transfer destination address width(bytes).
 * param dest_offset_minor destination address offset minor.
 * param dest_offset_major destination address offset major.
 * param bytesEachRequest eDMA transfer bytes per channel request.
 * param transferBytes eDMA transfer bytes to be transferred.
 * param enable_auto_stop_major true when auto stop enable
 * param channel channel number
 *
 * note The data address and the data width must be consistent. For example, if the SRC
 *       is 4 bytes, the source address must be 4 bytes aligned, or it results in
 *       source address error (SAE).
 */
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
						 uint32_t channel)
{
	edma_transfer_config_t transfer_config;
	EDMA_PrepareTransferConfig(&transfer_config,
	                                src_Addr,
	                                src_size,
	                                src_offset_minor,
	                                dest_Addr,
									dest_size,
	                                dest_offset_minor,
	                                bytesEachRequest,
	                                transferBytes);

	EDMA_TcdSetMajorOffsetConfig(&g_tcdMemoryPoolPtr[channel], src_offset_major,  dest_offset_major);
	EDMA_TcdSetTransferConfig(&g_tcdMemoryPoolPtr[channel], &transfer_config, NULL);
	EDMA_TcdEnableAutoStopRequest(&g_tcdMemoryPoolPtr[channel], enable_auto_stop_major);
	EDMA_InstallTCD(DMA0, channel, &g_tcdMemoryPoolPtr[channel]);
	EDMA_EnableChannelRequest(DMA0, channel);
}

/*!
 * brief Configures the DMAMUX channel source.
 * param channel DMAMUX channel number.
 * param source Channel source, which is used to trigger the DMA transfer.
 * param period_trigger enables the DMAMUX period trigger feature.
 */
void set_dmamux(uint32_t channel, dmamux_src_t source, bool period_trigger)
{
	DMAMUX_Init(DMAMUX0);
	DMAMUX_SetSource(DMAMUX0, channel, (uint32_t)source);
	if(MAX_TRIGGER_CHANEL >= channel)
	{
		if(true == period_trigger)
		{
			DMAMUX_EnablePeriodTrigger(DMAMUX0, channel);
		}
		else
		{
			DMAMUX_DisablePeriodTrigger(DMAMUX0, channel);
		}
	}
	DMAMUX_EnableChannel(DMAMUX0, channel);
}

/*!
 * brief Enables the DMAMUX channel.
 * param channel DMAMUX channel number.
 * param enable Channel source, which is used to trigger the DMA transfer.
 * param enable Enables the DMAMUX channel.
 */
void enable_channel(uint32_t channel, bool enable)
{
	if(true == enable)
	{
		DMAMUX_EnableChannel(DMAMUX0, channel);
	}
	else
	{
		DMAMUX_DisableChannel(DMAMUX0, channel);
	}
}
