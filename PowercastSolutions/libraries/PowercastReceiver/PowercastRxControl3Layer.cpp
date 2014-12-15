#include "PowercastRxControl3Layer.h"


PowercastRxControl3Layer::PowercastRxControl3Layer()
{
	timerPing = 0;
	timerWaitingForSecondProbe = 0;
	currentState = STATE_IDLE;
	measurementObject = NULL;
}

void PowercastRxControl3Layer::resetMeasurement()
{
	//timerPing = 0;
	//timerWaitingForSecondProbe = 0;
	//currentState = STATE_IDLE;
	TxAdressesQueue newTxAddressQueue;
	txAddressQueue = newTxAddressQueue;
}

void PowercastRxControl3Layer::processPacket(ProtocolPacket *xBeePacket)
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
		else if (currentState == STATE_PWR_PROBING && txAddressQueue.checkForLastAddress(xBeePacket->senderAddress)) {
			if (voltageMeasurementStructData->calculatedVoltageMv < VOLTAGE_THRESHOLD) {
				//Serial.println(F("eventPowerProbeRequestReceivedFromLastProbeBelowTh"));
				eventPowerProbeRequestReceivedFromLastProbeBelowTh(xBeePacket);
			}
			else {
				//Serial.println(F("eventPowerProbeRequestReceivedFromLastProbeAboveTh"));
				eventPowerProbeRequestReceivedFromLastProbeAboveTh(xBeePacket);
			}
		}
		else if (currentState == STATE_CHARGING && txAddressQueue.checkForLastAddress(xBeePacket->senderAddress)) {
			//Serial.println(F("eventPowerProbeRequestReceivedFromLastProbeAboveTh"));
			eventPowerProbeRequestReceivedFromLastProbeAboveTh(xBeePacket);
		}
	}
}

void PowercastRxControl3Layer::eventTimeoutPing()
{
	if (measurementObject != NULL) {
		measurementObject->saveFsmEventToFile(IS_TX, EVENT_TIMEOUT_PING, currentState);
	}
	processState(EVENT_TIMEOUT_PING, NULL);
}

void PowercastRxControl3Layer::eventVoltageBelowThreshold()
{
	if (measurementObject != NULL) {
		measurementObject->saveFsmEventToFile(IS_TX, EVENT_VOLTAGE_BELOW_THRESHOLD, currentState);
	}
	processState(EVENT_VOLTAGE_BELOW_THRESHOLD, NULL);
}

void PowercastRxControl3Layer::eventPowerProbeRequestReceivedNotFromQueueAdress(ProtocolPacket *xBeePacket)
{
	if (measurementObject != NULL) {
		measurementObject->saveFsmEventToFile(IS_TX, EVENT_POWER_PROBE_REQUEST_RECEIVED_NOT_FROM_QUEUE_ADDRESS, currentState);
	}
	processState(EVENT_POWER_PROBE_REQUEST_RECEIVED_NOT_FROM_QUEUE_ADDRESS, xBeePacket);
}

void PowercastRxControl3Layer::eventPowerProbeRequestReceivedFromLastProbeBelowTh(ProtocolPacket *xBeePacket)
{
	if (measurementObject != NULL) {
		measurementObject->saveFsmEventToFile(IS_TX, EVENT_POWER_PROBE_REQUEST_RECEIVED_FROM_LAST_PROBE_BELOW_THRESHOLD, currentState);
	}
	processState(EVENT_POWER_PROBE_REQUEST_RECEIVED_FROM_LAST_PROBE_BELOW_THRESHOLD, xBeePacket);
}

void PowercastRxControl3Layer::eventPowerProbeRequestReceivedFromLastProbeAboveTh(ProtocolPacket *xBeePacket)
{
	if (measurementObject != NULL) {
		measurementObject->saveFsmEventToFile(IS_TX, EVENT_POWER_PROBE_REQUEST_RECEIVED_FROM_LAST_PROBE_ABOVE_THRESHOLD, currentState);
	}
	processState(EVENT_POWER_PROBE_REQUEST_RECEIVED_FROM_LAST_PROBE_ABOVE_THRESHOLD, xBeePacket);
}

void PowercastRxControl3Layer::eventTimeoutRemoveOldestProbeSender()
{
	if (measurementObject != NULL) {
		measurementObject->saveFsmEventToFile(IS_TX, EVENT_TIMEOUT_REMOVE_OLDEST, currentState);
	}
	processState(EVENT_TIMEOUT_REMOVE_OLDEST, NULL);
}

void PowercastRxControl3Layer::eventTimeoutWaitingForSecondProbe()
{
	if (measurementObject != NULL) {
		measurementObject->saveFsmEventToFile(IS_TX, EVENT_TIMEOUT_WAITING_FOR_SECOND_PROBE, currentState);
	}
	processState(EVENT_TIMEOUT_WAITING_FOR_SECOND_PROBE, NULL);
}


void PowercastRxControl3Layer::processState(Event currentEvent, ProtocolPacket *xBeePacket)
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
			actionSendPowerProbeResponse(xBeePacket);
			timerWaitingForSecondProbe = getCurrentTimeMiliseconds();
			nextState = STATE_PWR_PROBING;
		}
		else if (currentEvent == EVENT_TIMEOUT_REMOVE_OLDEST)
		{
			actionRemoveOldestPowerProbeAddress();
		}
		break;
	case STATE_CHARGING:
		if (currentEvent == EVENT_TIMEOUT_PING)
		{
			actionSendPing();
		}
		else if (currentEvent == EVENT_POWER_PROBE_REQUEST_RECEIVED_FROM_LAST_PROBE_ABOVE_THRESHOLD)
		{
				actionSendPowerProbeResponse(xBeePacket);
		}
		else if (currentEvent == EVENT_VOLTAGE_BELOW_THRESHOLD)
		{
			nextState = STATE_IDLE;
		}
		else if (currentEvent == EVENT_TIMEOUT_REMOVE_OLDEST)
		{
			actionRemoveOldestPowerProbeAddress();
		}
		break;
	case STATE_PWR_PROBING:
		if (currentEvent == EVENT_TIMEOUT_PING)
		{
			actionSendPing();
		}
		else if (currentEvent == EVENT_POWER_PROBE_REQUEST_RECEIVED_FROM_LAST_PROBE_BELOW_THRESHOLD)
		{
			actionSendPowerProbeResponse(xBeePacket);
			nextState = STATE_IDLE;
		}
		else if (currentEvent == EVENT_POWER_PROBE_REQUEST_RECEIVED_FROM_LAST_PROBE_ABOVE_THRESHOLD)
		{
			actionSendPowerProbeResponse(xBeePacket);
			nextState = STATE_CHARGING;
		}
		else if (currentEvent == EVENT_TIMEOUT_WAITING_FOR_SECOND_PROBE)
		{
			nextState = STATE_IDLE;
		}
		else if (currentEvent == EVENT_TIMEOUT_REMOVE_OLDEST)
		{
			actionRemoveOldestPowerProbeAddress();
		}
		break;
	default:
		break;
	}

	//Serial.println(F("------------------------------")); 
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

void PowercastRxControl3Layer::doTimerActions()
{
	//Serial.println(".");
	//Serial.println(millis());
	unsigned long currentTime;
	currentTime = getCurrentTimeMiliseconds();
	if (currentTime - timerPing >= DELAY_PING) {
		eventTimeoutPing();
		timerPing = getCurrentTimeMiliseconds();;
	}

	if (txAddressQueue.checkOldestTxForTimeout())
	{
		eventTimeoutRemoveOldestProbeSender();
	}
	

	if (currentState == STATE_PWR_PROBING) {
		currentTime = getCurrentTimeMiliseconds();
		if (currentTime - timerWaitingForSecondProbe >= DELAY_WAITING_FOR_SECOND_PROBE) {
			eventTimeoutWaitingForSecondProbe();
		}
	}
	
	//Serial.println(voltageMeasurementStructData->calculatedVoltageMv);
	if (currentState == STATE_CHARGING) {
		if (voltageMeasurementStructData->calculatedVoltageMv < VOLTAGE_THRESHOLD) {
			eventVoltageBelowThreshold();
		}
	}

	doMeasurement();
}


void  PowercastRxControl3Layer::actionSendPing()
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

	//responsePacket.packetData = NULL;
	responsePacket.packetDataLength = 0;

	if (measurementObject != NULL) {
		measurementObject->savePacketEventToFile(IS_TX, false, &responsePacket);
	}
	if (currentState == STATE_CHARGING) {
		parentLayer->sendPacketToAddress(&responsePacket, txAddressQueue.getLastAddress());
	}
	else
	{
		parentLayer->sendPacketBroadcast(&responsePacket);
	}

}

void PowercastRxControl3Layer::actionSendPowerProbeResponse(ProtocolPacket *xBeePacket)
{
	if (measurementObject != NULL) {
		measurementObject->saveFsmActionToFile(IS_TX, ACTION_SEND_POWER_PROBE_RESPONSE, currentState);
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

void  PowercastRxControl3Layer::actionSaveReceivedPowerProbeAddress(ProtocolPacket *xBeePacket)
{
	if (measurementObject != NULL) {
		measurementObject->saveFsmActionToFile(IS_TX, ACTION_SAVE_RECEIVED_POWER_PROBE_ADDRESS, currentState);
	}

	txAddressQueue.saveAddressToQueue(xBeePacket->senderAddress);
}

void  PowercastRxControl3Layer::actionRemoveOldestPowerProbeAddress()
{
	if (measurementObject != NULL) {
		measurementObject->saveFsmActionToFile(IS_TX, ACTION_REMOVE_OLDEST_POWER_PROBE_ADDRESS, currentState);
	}

	txAddressQueue.removeOldestFromQueue();
}


void PowercastRxControl3Layer::doMeasurement()
{
	if (measurementObject != NULL) {
		measurementObject->updateReceiverData(voltageMeasurementStructData->calculatedVoltageMv);
	}
}
