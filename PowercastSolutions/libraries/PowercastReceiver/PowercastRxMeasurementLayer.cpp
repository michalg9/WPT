#include "PowercastRxMeasurementLayer.h"

//extern ExperimentThresholds experimentThresholds;

PowercastRxMeasurementLayer::PowercastRxMeasurementLayer()
{
	timerTimeSynch = 0;
	timerMeasurement = 0;
	currentState = STATE_IDLE;
	measurementObject = NULL;
	firstTimeSynchSent = false;
}

void PowercastRxMeasurementLayer::setMeasurementObject(MeasurementObject *measObject)
{
	measurementObject = measObject;
}

void PowercastRxMeasurementLayer::processPacket(ProtocolPacket *xBeePacket)
{
	//if (xBeePacket->packetType == TYPE_SET_COMM_THRESHOLD) {
	//	experimentThresholds.setCommThreshold(xBeePacket->packetData[0]);
	//}
	//else if (xBeePacket->packetType == TYPE_SET_POWER_THRESHOLD) {
	//	int voltageThreshold = makeInteger(xBeePacket->packetData[0], xBeePacket->packetData[1]);
	//	experimentThresholds.setPowerThreshold(voltageThreshold);
	//}
}

void PowercastRxMeasurementLayer::eventTimeoutTimeSynch()
{
	processState(EVENT_TIMEOUT_TIME_SYNCH, NULL);
}

void PowercastRxMeasurementLayer::eventTimeoutMeasurement()
{
	processState(EVENT_TIMEOUT_MEASUREMENT, NULL);
}


void PowercastRxMeasurementLayer::processState(Event currentEvent, ProtocolPacket *xBeePacket)
{
	//Serial.println("processState");

	
	int nextState = currentState;
	switch (currentState)
	{
	case STATE_IDLE:
		if (currentEvent == EVENT_TIMEOUT_TIME_SYNCH)
		{
			actionSendTimeSynchBroadcast();
		}
		else if (currentEvent == EVENT_TIMEOUT_MEASUREMENT)
		{
			actionSaveResultsToSd();
		}
		break;
	default:
		break;
	}
}


void PowercastRxMeasurementLayer::doTimerActions()
{
	unsigned long currentTime = getCurrentTimeMiliseconds();
	
	if (parentLayer->enableSending && !firstTimeSynchSent) {
		eventTimeoutTimeSynch();
		timerTimeSynch = currentTime;
		firstTimeSynchSent = true;
	}

	if (currentTime - timerTimeSynch >= DELAY_TIME_SYNCH) {
		eventTimeoutTimeSynch();
		timerTimeSynch = currentTime;
	}
	

	if (currentTime - timerMeasurement >= DELAY_MEASUREMENT) {
		eventTimeoutMeasurement();
		timerMeasurement = currentTime;
	}
}

void  PowercastRxMeasurementLayer::actionSendTimeSynchBroadcast()
{
	// Frame format:
	// 1st byte - PROTOCOL
	// 2st byte - TYPE
	// 3nd, 4rd, 5th and 6th byte - timestamp
	ProtocolPacket timeSynchPacket;

	timeSynchPacket.protocolType = PROTOCOL_TIMESYNCH;
	timeSynchPacket.packetType = TYPE_TIME_SYNCH;
	
	byte byteArray[4];
	unsigned long currentTimestamp = millis();
	convertLongToByteArray(currentTimestamp, byteArray);

	memcpy(timeSynchPacket.packetData, byteArray, 4);
	timeSynchPacket.packetDataLength = 4;

	parentLayer->sendPacketBroadcast(&timeSynchPacket);
	
}

void PowercastRxMeasurementLayer::actionSaveResultsToSd() {
	measurementObject->saveReceiverDataToFile();
}

void PowercastRxMeasurementLayer::convertLongToByteArray(long longInt, byte byteArray[]) {

	byteArray[0] = (byte)((longInt >> 24) & 0xFF);
	byteArray[1] = (byte)((longInt >> 16) & 0xFF);
	byteArray[2] = (byte)((longInt >> 8) & 0XFF);
	byteArray[3] = (byte)((longInt & 0XFF));

}

long PowercastRxMeasurementLayer::convertByteArrayToLong(byte byteArray[]) {
	unsigned long longInt;

	longInt = ((unsigned long)byteArray[0]) << 24;
	longInt |= ((unsigned long)byteArray[1]) << 16;
	longInt |= ((unsigned long)byteArray[2]) << 8;
	longInt |= ((unsigned long)byteArray[3]);

	return longInt;
}