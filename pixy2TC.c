


#include "pixy2TC.h"

static volatile uint8_t g_buffPayloadTemplateClass[3] = {0};

static volatile transferSend_t g_templateClassTrasferSendConfig = {
		PIXY_NO_CHECKSUM_SYNC & 0xFF,
		(PIXY_NO_CHECKSUM_SYNC >> 8) & 0xFF,
		0,
		0,
		(uint8_t*)g_buffPayloadTemplateClass
};
static volatile transferRecv_t g_templateClassTrasferRecvConfig = {0};


bool setLamp(bool upper, bool lower)
{
	bool retval = false;
	status_t status = kStatus_Success;
	g_templateClassTrasferSendConfig.typePack 	= PIXY_TYPE_REQUEST_LAMP;
	g_templateClassTrasferSendConfig.payLoadLen = 2;
	g_templateClassTrasferSendConfig.buffPayload[0] = upper;
	g_templateClassTrasferSendConfig.buffPayload[1] = lower;

	status = sendTrasferConfig_Pixy2(g_templateClassTrasferSendConfig);
	if(kStatus_Success == status)
	{
		status = recvTrasferConfig_Pixy2(&g_templateClassTrasferRecvConfig);
	}

	if(kStatus_Success == status)
	{
		if(PIXY_TYPE_RESPONSE_RESULT == g_templateClassTrasferRecvConfig.typePack)
		{
			if(g_templateClassTrasferRecvConfig.payLoadLen == 4)
			{
				retval = true;
			}
		}
	}
	return retval;
}

