/*
 * todo
 * 		All transfers are made without checksum. Modify for future use.
 */


#ifndef PIXY2_H_
#define PIXY2_H_

#include "i2c.h"

#define PIXY_BUFFERSIZE_TX 		6
#define PIXY_BUFFERSIZE_RX 		100
#define PIXY_CHECKSUM_SYNC 		0xc1af
#define PIXY_NO_CHECKSUM_SYNC 	0xc1ae


#define PIXY_BAUDRATE			100000
#define PIXY_I2C_SLAVE_ADDR		0x54

typedef struct _transferSend
{
	uint8_t bit1_checkSumSync;
	uint8_t bit2_checkSumSync;
	uint8_t typePack;
	uint8_t payLoadLen;
	uint8_t *buffPayload;
}transferSend_t;

typedef struct _transferRecv
{
	uint8_t bit1_checkSumSync;
	uint8_t bit2_checkSumSync;
	uint8_t typePack;
	uint8_t payLoadLen;
	uint16_t checkSum;
	uint8_t buffPayload;
}transferRecv_t;


typedef transferSend_t  * trasnfSnd_t;
typedef transferRecv_t  * trasnfRecv_t;

uint8_t Init_Pixy2();
uint8_t setSendTrasferConf_Pixy2(transferSend_t transferConfig);
uint8_t* getBufferPayloadAddr();
uint8_t sendTrasferConfig_Pixy2();

#endif /* PIXY2_H_ */
