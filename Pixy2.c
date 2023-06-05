/*
 * todo
 * 		-Return errors not implemented yet.
 * 		-Just works with vector request.
 */

#include "Pixy2.h"


static volatile uint8_t g_buffer_tx[PIXY_BUFFERSIZE_TX] = {0};
static volatile uint8_t g_buffer_rx[PIXY_BUFFERSIZE_RX] =
{
		1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22
};

static volatile trasnfSnd_t  g_buffer_tx_ptr 	= (trasnfSnd_t)g_buffer_tx;
static volatile trasnfRecv_t g_buffer_rx_ptr 	= (trasnfRecv_t)g_buffer_rx;

//Buffer gets transformed in single variable to perform a read
static volatile uint64_t g_subAddres = 0;

volatile static i2c_master_transfer_t g_pixy2TrasferMasterI2C =
{
		kI2C_TransferDefaultFlag,
		PIXY_I2C_SLAVE_ADDR,
		kI2C_Read,
		0,
		PIXY_BUFFERSIZE_TX,
		g_buffer_rx,
		PIXY_BUFFERSIZE_RX
};

uint8_t Init_Pixy2()
{
	I2C_init(I2C_0, PIXY_BAUDRATE);
	return 0;
}

uint8_t setSendTrasferConf_Pixy2(transferSend_t transferConfig)
{
	g_buffer_tx_ptr->bit1_checkSumSync 	= transferConfig.bit1_checkSumSync;
	g_buffer_tx_ptr->bit2_checkSumSync 	= transferConfig.bit2_checkSumSync;
	g_buffer_tx_ptr->typePack			= transferConfig.typePack;
	g_buffer_tx_ptr->payLoadLen			= transferConfig.payLoadLen;

	uint8_t * ptrBuffPayLoad = (uint8_t*)&g_buffer_tx_ptr->buffPayload;
	uint8_t index = 0;
	for(index = 0; index < transferConfig.payLoadLen; index++)
	{
		*(ptrBuffPayLoad+index) = *(transferConfig.buffPayload + index);
	}

	return 0;
}

uint8_t* getBufferPayloadAddr()
{
	return ((uint8_t*)&g_buffer_rx_ptr->buffPayload);
}

uint8_t sendTrasferConfig_Pixy2()
{
	// todo transfer function
	status_t retval = kStatus_Success;

	volatile uint8_t index = 0;
	g_subAddres = 0;
	for(index = 0; index < PIXY_BUFFERSIZE_TX; index++)
	{
		g_subAddres = g_subAddres << 8;
		g_subAddres |= g_buffer_tx[index];
	}

	g_pixy2TrasferMasterI2C.subaddress = g_subAddres;
	retval = I2C_ReadBlocking(I2C_0, &g_pixy2TrasferMasterI2C);
	return retval;
}

