#include "PowercastTxMeasurementLayer.h"


PowercastTxMeasurementLayer::PowercastTxMeasurementLayer()
{
	timerMeasurement = 0;
	currentState = STATE_IDLE;
	measurementObject = NULL;
}

void PowercastTxMeasurementLayer::setMeasurementObject(MeasurementObject *measObject)
{
	measurementObject = measObject;
}

void PowercastTxMeasurementLayer::processPacket(ProtocolPacket *xBeePacket)
{
	//Serial.println("processPacket");

	if (xBeePacket->packetType == TYPE_TIME_SYNCH) {
		eventReceivedTimeSynch(xBeePacket);
	}
}

void PowercastTxMeasurementLayer::eventReceivedTimeSynch(ProtocolPacket *xBeePacket)
{
	processState(EVENT_RECEIVED_TIME_SYNCH, xBeePacket);
}

void PowercastTxMeasurementLayer::eventTimeoutMeasurement()
{
	processState(EVENT_TIMEOUT_MEASUREMENT, NULL);
}


void PowercastTxMeasurementLayer::processState(Event currentEvent, ProtocolPacket *xBeePacket)
{
	//Serial.println("processState");

	
	int nextState = currentState;
	switch (currentState)
	{
	case STATE_IDLE:
		if (currentEvent == EVENT_RECEIVED_TIME_SYNCH)
		{
			actionSaveTimeSynch(xBeePacket);
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

void PowercastTxMeasurementLayer::doTimerActions()
{
	unsigned long currentTime = getCurrentTimeMiliseconds();
	if (currentTime - timerMeasurement >= DELAY_MEASUREMENT) {
		eventTimeoutMeasurement();
		timerMeasurement = currentTime;
	}
}

void PowercastTxMeasurementLayer::actionSaveTimeSynch(ProtocolPacket *xBeePacket)
{
	byte byteArray[4];

	byteArray[0] = xBeePacket->packetData[0];
	byteArray[1] = xBeePacket->packetData[1];
	byteArray[2] = xBeePacket->packetData[2];
	byteArray[3] = xBeePacket->packetData[3];

	unsigned long longInt = convertByteArrayToLong(byteArray);

	measurementObject->recentReceivedTimestamp = longInt;
	measurementObject->localTimestampOnSynch = getCurrentTimeMiliseconds();

	/*Serial.println("Received timestamp synch:");
	Serial.println(measurementObject->recentReceivedTimestamp);
	Serial.println("Current timestamp:");
	Serial.println(measurementObject->localTimestampOnSynch);*/
}

void PowercastTxMeasurementLayer::actionSaveResultsToSd() {
	measurementObject->saveTransmitterDataToFile();
}


//void PowercastTxMeasurementLayer::convertLongToByteArray(long longInt, byte byteArray[]) {
//
//	byteArray[0] = (byte)((longInt >> 24) & 0xFF);
//	byteArray[1] = (byte)((longInt >> 16) & 0xFF);
//	byteArray[2] = (byte)((longInt >> 8) & 0XFF);
//	byteArray[3] = (byte)((longInt & 0XFF));
//
//}

long PowercastTxMeasurementLayer::convertByteArrayToLong(byte byteArray[]) {
	unsigned long longInt;

	longInt = ((unsigned long)byteArray[0]) << 24;
	longInt |= ((unsigned long)byteArray[1]) << 16;
	longInt |= ((unsigned long)byteArray[2]) << 8;
	longInt |= ((unsigned long)byteArray[3]);

	return longInt;
}