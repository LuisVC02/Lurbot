/*
 * todo
 * 		getMainFeatures_LinePixy2 just works to ask for vectors. Is left
 * 		intersections and barcode.
 */
#include "Pixy2LineTracking.h"

static volatile featureTypeBuff_t * g_vectorFeature = 0;
static volatile vector_t * g_vectors_ptr = 0;

const uint8_t buffPayloadLineTracking[2] = {
		LINE_GET_ALL_FEATURES,
		LINE_VECTOR
};

featureTypeBuff_t* getMainFeatures_LinePixy2(){

	static transferSend_t lineTrackingTrasferConfig = {
			PIXY_NO_CHECKSUM_SYNC & 0xFF,
			(PIXY_NO_CHECKSUM_SYNC >> 8) & 0xFF,
			LINE_REQUEST_GET_FEATURES,
			2,
			(uint8_t*)buffPayloadLineTracking
	};


	setSendTrasferConf_Pixy2(lineTrackingTrasferConfig);
	g_vectorFeature = (featureTypeBuff_t*)getBufferPayloadAddr();
	g_vectors_ptr 	= (vector_t*)&g_vectorFeature->featureData;

	sendTrasferConfig_Pixy2();
	return (featureTypeBuff_t*)g_vectorFeature;
}



