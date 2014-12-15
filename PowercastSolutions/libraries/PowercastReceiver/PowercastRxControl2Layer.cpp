#include "PowercastRxControl2Layer.h"


PowercastRxControl2Layer::PowercastRxControl2Layer()
{
	timerPing = 0;
	currentState = STATE_IDLE;
	measurementObject = NULL;
}

void PowercastRxControl2Layer::processPacket(ProtocolPacket *xBeePacket)
{
	//Serial.println("processPacket");

	if (xBeePacket->packetType == TYPE_TEST) {
		//Serial.println("processPacket - eventReceivedTestPacket");
		eventReceivedTestPacket(xBeePacket);
	}
	else if (xBeePacket->packetType == TYPE_VOLTAGE_REQUEST) {
		//Serial.rintln("processPacket - eventReceivedVoltagePacket");
		eventReceivedVoltagePacket(xBeePacket);
	}
}


void PowercastRxControl2Layer::eventReceivedTestPacket(ProtocolPacket *xBeePacket)
{
	if (measurementObject != NULL) {
		measurementObject->savePacketEventToFile(IS_TX, true, xBeePacket);
	}

	processState(EVENT_TEST_REQUEST_RECEIVED, xBeePacket);
}


void PowercastRxControl2Layer::eventTimeoutPing()
{
	if (measurementObject != NULL) {
		measurementObject->saveFsmEventToFile(IS_TX, EVENT_TIMEOUT_PING, currentState);
	}
	processState(EVENT_TIMEOUT_PING, NULL);
}

void PowercastRxControl2Layer::eventReceivedVoltagePacket(ProtocolPacket *xBeePacket)
{
	if (measurementObject != NULL) {
		measurementObject->savePacketEventToFile(IS_TX, true, xBeePacket);
	}
	processState(EVENT_VOLTAGE_REQUEST_RECEIVED, xBeePacket);
}

void PowercastRxControl2Layer::processState(Event currentEvent, ProtocolPacket *xBeePacket)
{
	//Serial.println("processState");


	int nextState = currentState;
	switch (currentState)
	{
	case STATE_IDLE:
		if (currentEvent == EVENT_TIMEOUT_PING)
		{
			actionSendPing();
		}
		else if (currentEvent == EVENT_TEST_REQUEST_RECEIVED)
		{

			//Serial.println("processState - actionSendRepsonse");
			actionSendTestResponse(xBeePacket);
		}
		else if (currentEvent == EVENT_VOLTAGE_REQUEST_RECEIVED)
		{
			//Serial.println("processState - actionSendVoltageRepsonse");
			actionSendVoltageResponse(xBeePacket);
		}
		nextState = currentState;
		break;
	default:
		break;
	}

	currentState = nextState;
}


void PowercastRxControl2Layer::doTimerActions()
{
	unsigned long currentTime = getCurrentTimeMiliseconds();
	if (millis() - timerPing >= DELAY_PING) {
		eventTimeoutPing();
		timerPing = currentTime;
	}

	doMeasurement();
}

void PowercastRxControl2Layer::actionSendTestResponse(ProtocolPacket *xBeePacket)
{

	// Frame format:
	// 1st byte - protocol type
	// 1st byte - packet type
	// 2nd byte - request sequenceNumber
	// remaining bytes - content of request packet
	ProtocolPacket responsePacket;
	responsePacket.protocolType = PROTOCOL_CONTROL;
	responsePacket.packetType = TYPE_TEST_RESPONSE;
	memcpy(responsePacket.packetData, xBeePacket->packetData, xBeePacket->packetDataLength);
	//responsePacket.packetData = xBeePacket.packetData;
	responsePacket.packetDataLength = xBeePacket->packetDataLength;

	//Serial.println("actionSendRepsonse");
	//Serial.println(xBeePacket.senderAddress, HEX);
	if (measurementObject != NULL) {
		measurementObject->savePacketEventToFile(IS_TX, false, &responsePacket);
	}

	parentLayer->sendPacketToAddress(&responsePacket, xBeePacket->senderAddress);

}


void  PowercastRxControl2Layer::actionSendPing()
{
	// Frame format:
	// 1st byte - PROTOCOL
	// 2st byte - TYPE
	ProtocolPacket responsePacket;

	responsePacket.protocolType = PROTOCOL_CONTROL;
	responsePacket.packetType = TYPE_PING;

	//responsePacket.packetData = NULL;
	responsePacket.packetDataLength = 0;

	if (measurementObject != NULL) {
		measurementObject->savePacketEventToFile(IS_TX, false, &responsePacket);
	}

	parentLayer->sendPacketBroadcast(&responsePacket);

}

void PowercastRxControl2Layer::actionSendVoltageResponse(ProtocolPacket *xBeePacket)
{
	// Frame format:
	// 1st byte - protocol type
	// 1st byte - packet type
	// 2nd byte - request sequenceNumber
	// remaining bytes - content of request packet
	ProtocolPacket responsePacket;
	responsePacket.protocolType = PROTOCOL_CONTROL;
	responsePacket.packetType = TYPE_VOLTAGE_RESPONSE;

	// Frame format:
	// 1st byte - sequence number
	// 2nd and 3rd byte - voltage [mV]
	// 4th and 5th byte - sample number (0-1023)
	// 6th and 7th - voltage reference used [mV]
	byte payload[] = { 0, 0, 0, 0, 0, 0, 0 };
	payload[0] = xBeePacket->packetData[0];
	payload[1] = highByte(voltageMeasurementStructData->calculatedVoltageMv);
	payload[2] = lowByte(voltageMeasurementStructData->calculatedVoltageMv);
	payload[3] = highByte(voltageMeasurementStructData->usedSampleValue);
	payload[4] = lowByte(voltageMeasurementStructData->usedSampleValue);
	payload[5] = highByte(voltageMeasurementStructData->referenceValMv);
	payload[6] = lowByte(voltageMeasurementStructData->referenceValMv);


	memcpy(responsePacket.packetData, payload, 7);
	responsePacket.packetDataLength = 7;


	if (measurementObject != NULL) {
		measurementObject->savePacketEventToFile(IS_TX, false, &responsePacket);
	}

	Serial.print("VOLTAGE ");
	Serial.print(voltageMeasurementStructData->calculatedVoltageMv, DEC);
	Serial.print(" VOLTAGE RESPONSE TO ADDRESS ");
	Serial.println(xBeePacket->senderAddress, HEX);


	parentLayer->sendPacketToAddress(&responsePacket, xBeePacket->senderAddress);

}

void PowercastRxControl2Layer::doMeasurement()
{
	if (measurementObject != NULL) {
		measurementObject->updateReceiverData(voltageMeasurementStructData->calculatedVoltageMv);
	}
}