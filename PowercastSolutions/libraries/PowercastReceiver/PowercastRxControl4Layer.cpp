#include "PowercastRxControl4Layer.h"

PowercastRxControl4Layer::PowercastRxControl4Layer()
{
	timerPing = 0;
	timerProbing = 0;
	timerWaitingForPower = 0;
	currentState = STATE_IDLE;
	measurementObject = NULL;
}

void PowercastRxControl4Layer::resetMeasurement()
{
	//timerPing = 0;
	//timerWaitingForSecondProbe = 0;
	//currentState = STATE_IDLE;
	txAddressQueue.resetQueue();
}

void PowercastRxControl4Layer::processPacket(ProtocolPacket *xBeePacket)
{
	//Serial.println(F("processPacket"));

	//txAddressQueue.printQueue();
	if (xBeePacket->packetType == TYPE_VOLTAGE_REQUEST) {

		if (measurementObject != NULL) {
			measurementObject->savePacketEventToFile(IS_TX, true, xBeePacket);
		}

		//Serial.println(F("voltageRequest"));
		//Serial.print("currentState: ");
		//Serial.println(currentState);
		if (currentState == STATE_IDLE && !txAddressQueue.checkForAddress(xBeePacket->senderAddress))  {
			//Serial.println(F("eventPowerProbeRequestReceivedNotFromQueueAdress"));
			eventPowerProbeRequestReceivedNotFromQueueAdress(xBeePacket);
		}
	}
}

void PowercastRxControl4Layer::eventTimeoutPing()
{
	if (measurementObject != NULL) {
		measurementObject->saveFsmEventToFile(IS_TX, EVENT_TIMEOUT_PING, currentState);
	}
	processState(EVENT_TIMEOUT_PING, NULL);
}


void PowercastRxControl4Layer::eventPowerProbeRequestReceivedNotFromQueueAdress(ProtocolPacket *xBeePacket)
{
	if (measurementObject != NULL) {
		measurementObject->saveFsmEventToFile(IS_TX, EVENT_POWER_PROBE_REQUEST_RECEIVED_NOT_FROM_QUEUE_ADDRESS, currentState);
	}
	processState(EVENT_POWER_PROBE_REQUEST_RECEIVED_NOT_FROM_QUEUE_ADDRESS, xBeePacket);
}

void PowercastRxControl4Layer::eventVoltageAboveThreshold()
{
	if (measurementObject != NULL) {
		measurementObject->saveFsmEventToFile(IS_TX, EVENT_VOLTAGE_ABOVE_THRESHOLD, currentState);
	}
	processState(EVENT_VOLTAGE_ABOVE_THRESHOLD, NULL);
}

void PowercastRxControl4Layer::eventTimeoutPowerProbing()
{
	if (measurementObject != NULL) {
		measurementObject->saveFsmEventToFile(IS_TX, EVENT_TIMEOUT_POWER_PROBING, currentState);
	}
	processState(EVENT_TIMEOUT_POWER_PROBING, NULL);
}

void PowercastRxControl4Layer::eventTimeoutWaitingForPower()
{
	if (measurementObject != NULL) {
		measurementObject->saveFsmEventToFile(IS_TX, EVENT_TIMEOUT_WAITING_FOR_POWER, currentState);
	}
	processState(EVENT_TIMEOUT_WAITING_FOR_POWER, NULL);
}

void PowercastRxControl4Layer::eventTimeoutRemoveOldestProbeSender()
{
	if (measurementObject != NULL) {
		measurementObject->saveFsmEventToFile(IS_TX, EVENT_TIMEOUT_REMOVE_OLDEST, currentState);
	}
	processState(EVENT_TIMEOUT_REMOVE_OLDEST, NULL);
}


void PowercastRxControl4Layer::eventVoltageBelowThreshold()
{
	if (measurementObject != NULL) {
		measurementObject->saveFsmEventToFile(IS_TX, EVENT_VOLTAGE_BELOW_THRESHOLD, currentState);
	}
	processState(EVENT_VOLTAGE_BELOW_THRESHOLD, NULL);
}


void PowercastRxControl4Layer::processState(Event currentEvent, ProtocolPacket *xBeePacket)
{

	int nextState = currentState;
	switch (currentState)
	{
	case STATE_IDLE:
		if (currentEvent == EVENT_TIMEOUT_PING)
		{
			actionSendPing();
		}
		else if (currentEvent == EVENT_POWER_PROBE_REQUEST_RECEIVED_NOT_FROM_QUEUE_ADDRESS)
		{
			actionSaveReceivedPowerProbeAddress(xBeePacket);
			actionSendPowerProbe(xBeePacket);
			timerWaitingForPower = getCurrentTimeMiliseconds();
			nextState = STATE_WAITING_FOR_POWER;
		}
		else if (currentEvent == EVENT_TIMEOUT_REMOVE_OLDEST)
		{
			actionRemoveOldestPowerProbeAddress();
		}
		break;
	case STATE_WAITING_FOR_POWER:
		if (currentEvent == EVENT_TIMEOUT_WAITING_FOR_POWER)
		{
			nextState = STATE_IDLE;
		}
		else if (currentEvent == EVENT_VOLTAGE_ABOVE_THRESHOLD)
		{
			actionSendPowerProbe(NULL);
			nextState = STATE_CHARGING;
		}
		else if (currentEvent == EVENT_TIMEOUT_REMOVE_OLDEST)
		{
			actionRemoveOldestPowerProbeAddress();
		}
		break;
	case STATE_CHARGING:
		if (currentEvent == EVENT_TIMEOUT_POWER_PROBING)
		{
			actionSendPowerProbe(NULL);
		}
		else if (currentEvent == EVENT_VOLTAGE_BELOW_THRESHOLD)
		{
			nextState = STATE_IDLE;
		}

		break;
	default:
		break;
	}

	//for (int i = 1; i < 4; i++) {
	//	Serial.println(F("------------------------------"));
	//}
	//Serial.print(F("ps - "));
	//Serial.print(F("t: "));
	//Serial.print(millis());
	//Serial.print(F(" s: "));
	//Serial.print(currentState);
	//Serial.print(F(" e: "));
	//Serial.print(currentEvent);
	//Serial.print(F(" n: "));
	//Serial.println(nextState);
	//Serial.println(F("//----------------------------"));


	currentState = nextState;
}

bool PowercastRxControl4Layer::isTheAddressAllowed(uint16_t address)
{
	return !txAddressQueue.checkForAddress(address);
}


void PowercastRxControl4Layer::doTimerActions()
{
	//Serial.println(".");
	//Serial.println(millis());
	unsigned long currentTime;

	if (currentState == STATE_IDLE) {
		currentTime = getCurrentTimeMiliseconds();
		if (currentTime - timerPing >= RX4_DELAY_PING) {
			eventTimeoutPing();
			timerPing = getCurrentTimeMiliseconds();
		}
	}
	
	if (currentState != STATE_CHARGING) {
		if (txAddressQueue.checkOldestTxForTimeout())
		{
			eventTimeoutRemoveOldestProbeSender();
		}


	}
	

	if (currentState == STATE_WAITING_FOR_POWER) {
		currentTime = getCurrentTimeMiliseconds();
		if (currentTime - timerWaitingForPower >= RX4_DELAY_WAITING_FOR_POWER) {
			eventTimeoutWaitingForPower();
		}
		
		if (voltageMeasurementStructData->calculatedVoltageMv > RX4_VOLTAGE_THRESHOLD) {
			eventVoltageAboveThreshold();
		}
	}

	if (currentState == STATE_CHARGING) {

		currentTime = getCurrentTimeMiliseconds();
		if (currentTime - timerProbing >= RX4_DELAY_POWER_PROBING) {
			eventTimeoutPowerProbing();
			timerProbing = getCurrentTimeMiliseconds();
		}

		if (voltageMeasurementStructData->calculatedVoltageMv < RX4_VOLTAGE_THRESHOLD) {
			eventVoltageBelowThreshold();
		}
	}

	doMeasurement();
}


void  PowercastRxControl4Layer::actionSendPing()
{
	if (measurementObject != NULL) {
		measurementObject->saveFsmActionToFile(IS_TX, ACTION_SEND_PING, currentState);
	}

	// Frame format:
	// 1st byte - PROTOCOL
	// 2st byte - TYPE
	ProtocolPacket responsePacket;

	responsePacket.protocolType = PROTOCOL_CONTROL;
	responsePacket.packetType = TYPE_PING;

	// change for changing RSSI measurements
	byte payload[] = { TX4_RSSI_THRESHOLD };
	memcpy(responsePacket.packetData, payload, 1);
	responsePacket.packetDataLength = 1;

	// old version of ping
	//responsePacket.packetData = NULL;
	//responsePacket.packetDataLength = 0;

	if (measurementObject != NULL) {
		measurementObject->savePacketEventToFile(IS_TX, false, &responsePacket);
	}

	parentLayer->sendPacketBroadcast(&responsePacket);


}

void PowercastRxControl4Layer::actionSendPowerProbe(ProtocolPacket *xBeePacket)
{
	if (measurementObject != NULL) {
		measurementObject->saveFsmActionToFile(IS_TX, ACTION_SEND_POWER_PROBE, currentState);
	}

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
	if (xBeePacket != NULL) {
		payload[0] = xBeePacket->packetData[0];
	}
	else {
		payload[0] = 0;
	}
	payload[1] = highByte(voltageMeasurementStructData->calculatedVoltageMv);
	payload[2] = lowByte(voltageMeasurementStructData->calculatedVoltageMv);
	payload[3] = highByte(RX4_VOLTAGE_THRESHOLD);
	payload[4] = lowByte(RX4_VOLTAGE_THRESHOLD);
	//payload[3] = highByte(voltageMeasurementStructData->usedSampleValue);
	//payload[4] = lowByte(voltageMeasurementStructData->usedSampleValue);
	//payload[5] = highByte(voltageMeasurementStructData->referenceValMv);
	//payload[6] = lowByte(voltageMeasurementStructData->referenceValMv);


	memcpy(responsePacket.packetData, payload, 5);
	responsePacket.packetDataLength = 5;

	/*memcpy(responsePacket.packetData, payload, 7);
	responsePacket.packetDataLength = 7;*/


	if (measurementObject != NULL) {
		measurementObject->savePacketEventToFile(IS_TX, false, &responsePacket);
	}

	/*Serial.print("VOLTAGE ");
	Serial.print(voltageMeasurementStructData->calculatedVoltageMv, DEC);*/

	if (xBeePacket != NULL) {
		/*Serial.print(" VOLTAGE RESPONSE TO ADDRESS ");
		Serial.println(xBeePacket->senderAddress, HEX);*/
		parentLayer->sendPacketToAddress(&responsePacket, xBeePacket->senderAddress);
	}
	else {
		//Serial.print(" VOLTAGE RESPONSE TO LAST ADDRESS ");
		//Serial.println(txAddressQueue.getLastAddress(), HEX);
		parentLayer->sendPacketToAddress(&responsePacket, txAddressQueue.getLastAddress());
	}

}

void  PowercastRxControl4Layer::actionSaveReceivedPowerProbeAddress(ProtocolPacket *xBeePacket)
{
	if (measurementObject != NULL) {
		measurementObject->saveFsmActionToFile(IS_TX, ACTION_SAVE_RECEIVED_POWER_PROBE_ADDRESS, currentState);
	}

	txAddressQueue.saveAddressToQueue(xBeePacket->senderAddress);
}

void  PowercastRxControl4Layer::actionRemoveOldestPowerProbeAddress()
{
	if (measurementObject != NULL) {
		measurementObject->saveFsmActionToFile(IS_TX, ACTION_REMOVE_OLDEST_POWER_PROBE_ADDRESS, currentState);
	}

	txAddressQueue.removeOldestFromQueue();
}


void PowercastRxControl4Layer::doMeasurement()
{
	if (measurementObject != NULL) {
		measurementObject->updateReceiverData(voltageMeasurementStructData->calculatedVoltageMv);
	}
}
