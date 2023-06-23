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

static volatile transferSend_t * g_buffer_tx_ptr 	= (transferSend_t*)g_buffer_tx;
static volatile transferRecv_t * g_buffer_rx_ptr 	= (transferRecv_t*)g_buffer_rx;





volatile static i2c_master_transfer_t g_pixy2TrasferMasterI2C =
{
		kI2C_TransferDefaultFlag,		/*!< A transfer flag which controls the transfer. */
		PIXY_I2C_SLAVE_ADDR,            /*!< 7-bit slave address. */
		kI2C_Read,                      /*!< A transfer direction, read or write. */
		0,                              /*!< A sub address. Transferred MSB first. */
		0,             					/*!< A size of the command buffer. */
		(uint8_t*)g_buffer_tx,          /*!< A transfer buffer. */
		PIXY_SEND_HEADER_SIZE              /*!< A transfer size. */
};


uint8_t Init_Pixy2()
{
	I2C_init(I2C_0, PIXY_BAUDRATE);
	return 0;
}

status_t sendTrasferConfig_Pixy2(transferSend_t transferConfig)
{
	status_t retval = kStatus_Success;

	g_buffer_tx_ptr->bit1_checkSumSync 	= transferConfig.bit1_checkSumSync;
	g_buffer_tx_ptr->bit2_checkSumSync 	= transferConfig.bit2_checkSumSync;
	g_buffer_tx_ptr->typePack			= transferConfig.typePack;
	g_buffer_tx_ptr->payLoadLen			= transferConfig.payLoadLen;

	uint8_t * ptrBuffPayLoad = (uint8_t*)&g_buffer_tx_ptr->buffPayload;
	uint8_t index = 0;
	for(index = 0; index < transferConfig.payLoadLen; index++)
	{
		ptrBuffPayLoad[index] = transferConfig.buffPayload[index];
	}

	g_pixy2TrasferMasterI2C.direction 	= kI2C_Write;
	g_pixy2TrasferMasterI2C.data 		= (uint8_t*)g_buffer_tx;
	g_pixy2TrasferMasterI2C.dataSize 	= PIXY_SEND_HEADER_SIZE + transferConfig.payLoadLen;
	retval = I2C_ReadBlocking(I2C_0, (i2c_master_transfer_t*)&g_pixy2TrasferMasterI2C);
	return retval;
}

status_t recvTrasferConfig_Pixy2(transferRecv_t* transferConfig)
{
	status_t retval 			= kStatus_Success;
	uint8_t * buffPayLoadPtr 	= NULL;

	g_pixy2TrasferMasterI2C.direction 	= kI2C_Read ;
	g_pixy2TrasferMasterI2C.data 		= (uint8_t*)g_buffer_rx;
	g_pixy2TrasferMasterI2C.dataSize 	= PIXY_SEND_HEADER_SIZE;
	retval = I2C_ReadBlocking(I2C_0, (i2c_master_transfer_t*)&g_pixy2TrasferMasterI2C);

	if(kStatus_Success == retval)
	{
		if(g_buffer_rx_ptr->payLoadLen > 0)
		{
			g_pixy2TrasferMasterI2C.data 		= (uint8_t*)(g_buffer_rx + PIXY_SEND_HEADER_SIZE);
			g_pixy2TrasferMasterI2C.dataSize 	= g_buffer_rx_ptr->payLoadLen + PIXY_BYTES_OF_DATA_CHECKSUM;
			if(g_pixy2TrasferMasterI2C.dataSize > (PIXY_BUFFERSIZE_RX-PIXY_SEND_HEADER_SIZE))
			{
				g_pixy2TrasferMasterI2C.dataSize = PIXY_BUFFERSIZE_RX-PIXY_SEND_HEADER_SIZE;
			}
			retval = I2C_ReadBlocking(I2C_0, (i2c_master_transfer_t*)&g_pixy2TrasferMasterI2C);
		}
	}

	if(kStatus_Success == retval)
	{
		if(g_buffer_rx_ptr->payLoadLen > 0)
		{
			transferConfig->bit1_checkSumSync 	=  g_buffer_rx_ptr->bit1_checkSumSync;
			transferConfig->bit2_checkSumSync 	=  g_buffer_rx_ptr->bit2_checkSumSync;
			transferConfig->typePack 			=  g_buffer_rx_ptr->typePack;
			transferConfig->payLoadLen 			=  g_buffer_rx_ptr->payLoadLen;
			transferConfig->checkSum 			=  g_buffer_rx_ptr->checkSum;

			buffPayLoadPtr = (uint8_t*)&g_buffer_rx_ptr->checkSum;
			transferConfig->buffPayload 		=  buffPayLoadPtr + PIXY_BYTES_OF_DATA_CHECKSUM;
		}
	}

	return retval;
}



//uint8_t sendTrasferConfig_Pixy2()
//{
//	// todo transfer function
//	status_t retval = kStatus_Success;
//	uint8_t retry = 0;
//
//	g_pixy2TrasferMasterI2C.direction 		= kI2C_Read;
//	g_pixy2TrasferMasterI2C.subaddress 		= g_subAddres;
//	g_pixy2TrasferMasterI2C.subaddressSize 	= PIXY_BUFFERSIZE_TX;
//	g_pixy2TrasferMasterI2C.data 			= (uint8_t*)g_buffer_rx;
//	g_pixy2TrasferMasterI2C.dataSize		= PIXY_BUFFERSIZE_RX;
//
//	retval = I2C_ReadBlocking(I2C_0, (i2c_master_transfer_t*)&g_pixy2TrasferMasterI2C);
//
//
//
//	return retval;
//}

