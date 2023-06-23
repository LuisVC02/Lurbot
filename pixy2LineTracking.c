/*
 * todo
 * 		getMainFeatures_LinePixy2 just works to ask for vectors. Is left
 * 		intersections and barcode.
 */
#include "Pixy2LineTracking.h"

static volatile uint8_t buffPayloadLineTracking[2] = {
		LINE_GET_ALL_FEATURES,
		LINE_VECTOR
};

featureTypeBuff_t* getMainFeatures_LinePixy2(featureType_t featureType)
{
	static transferSend_t lineTrackingTrasferSendConfig = {
			PIXY_NO_CHECKSUM_SYNC & 0xFF,
			(PIXY_NO_CHECKSUM_SYNC >> 8) & 0xFF,
			LINE_REQUEST_GET_FEATURES,
			2,
			(uint8_t*)buffPayloadLineTracking
	};
	static volatile transferRecv_t lineTrackingTrasferRecvConfig = {0};

	volatile featureTypeBuff_t * featureTypePtr = NULL;
	status_t status = kStatus_Success;
//	uint8_t * buffPayloadPtr = NULL;

	lineTrackingTrasferSendConfig.buffPayload[1] = featureType;
	status = sendTrasferConfig_Pixy2(lineTrackingTrasferSendConfig);

	if(kStatus_Success == status)
	{
		status = recvTrasferConfig_Pixy2(&lineTrackingTrasferRecvConfig);
	}

	if(kStatus_Success == status)
	{
		if(LINE_RESPONSE_GET_FEATURES == lineTrackingTrasferRecvConfig.typePack)
		{
			if(lineTrackingTrasferRecvConfig.payLoadLen > 7)
			{
				featureTypePtr = (featureTypeBuff_t*)lineTrackingTrasferRecvConfig.buffPayload;
			}
		}
	}
	return(featureTypePtr);
}



