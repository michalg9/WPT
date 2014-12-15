#include "PowercastTxControl3Layer.h"


PowercastTxControl3Layer::PowercastTxControl3Layer()
{
	lastPowerProbeReceived = 0;
	powerProbeResponseTimer = 0;
	currentState = STATE_OFF;
	measurementObject = NULL;
}

void PowercastTxControl3Layer::setOutputPins(int pinTxOff, int pinTxOn)
{
	this->pinTxOff = pinTxOff;
	this->pinTxOn = pinTxOn;
}

void PowercastTxControl3Layer::processPacket(ProtocolPacket *xBeePacket)
{
	//Serial.println("processPacket");
	//Serial.println(xBeePacket->packetType);
	//Serial.println(currentState);
	if (currentState == STATE_OFF && xBeePacket->packetType == TYPE_PING) {

		if (measurementObject != NULL) {
			measurementObject->savePacketEventToFile(IS_TX, true, xBeePacket);
		}

		byte rssiThreshold = xBeePacket->packetData[0];

		Serial.print(F("RSSI threshold of communication "));
		Serial.println(rssiThreshold);
		Serial.print(F("RSSI of communication the packet"));
		Serial.println(xBeePacket->receivedPacketRssi);

		if (xBeePacket->receivedPacketRssi < rssiThreshold) {
			Serial.println(F("EVENT hurray!"));
			eventReceivedPing(xBeePacket);
		}
	}
	
	if ((currentState == STATE_OFF_POWER_PROBE || currentState == STATE_ON) && xBeePacket->packetType == TYPE_VOLTAGE_RESPONSE) {

		if (measurementObject != NULL) {
			measurementObject->savePacketEventToFile(IS_TX, true, xBeePacket);
		}

		int receivedVoltage = makeInteger(xBeePacket->packetData[1], xBeePacket->packetData[2]);
		int voltageThreshold = makeInteger(xBeePacket->packetData[3], xBeePacket->packetData[4]);

		Serial.print(F("Received voltage "));
		Serial.println(receivedVoltage);
		Serial.print(F("Voltage threshold "));
		Serial.println(voltageThreshold);

		lastPowerProbeReceived = getCurrentTimeMiliseconds();

		if (receivedVoltage > voltageThreshold) {
			Serial.println(F("ABOVE"));
			eventPwrProbeReceivedAboveTh();
		}
		else {
			Serial.println(F("BELOW"));
			eventPwrProbeReceivedBelowTh();
		}
	}
}

void PowercastTxControl3Layer::processState(Event currentEvent, ProtocolPacket *xBeePacket)
{
	//Serial.println("processStateEnter");
	//this->printState();
	int nextState = currentState;
	switch (currentState)
	{
	case STATE_OFF:
		if (currentEvent == EVENT_RECEIVED_PING)
		{
			actionSendPwrProbe(xBeePacket);
			nextState = STATE_OFF_POWER_PROBE;
		}
		break;
	case STATE_OFF_POWER_PROBE:
		if (currentEvent == EVENT_PWR_PROBE_RECEIVED_ABOVE_TH || currentEvent == EVENT_TIMEOUT_PWR_PROBE_RESPONSE)
		{
			actionTurnOff();
			nextState = STATE_OFF;
		}
		else if (currentEvent == EVENT_PWR_PROBE_RECEIVED_BELOW_TH)
		{
			actionTurnOn();
			lastPowerProbeReceived = getCurrentTimeMiliseconds() - (TX4_TIMEOUT_TX_PROBE_RECEIVE - TX4_TURN_OFF_IF_FIRST_PWR_PROBE_NOT_RECEIVED_AFTER);
			nextState = STATE_ON;
		}
	case STATE_ON:
		if (currentEvent == EVENT_TIMEOUT_ON_POWER_PROBE)
		{
			actionTurnOff();
			nextState = STATE_OFF;
		}
	default:
		break;
	}

	//Serial.println("------------------------------");
	//Serial.print("ps - ");
	//Serial.print("t: ");
	//Serial.print(millis() - measurementObject->localTimestampOnSynch + measurementObject->recentReceivedTimestamp);
	//Serial.print(" s: ");
	//Serial.print(currentState);
	//Serial.print(" e: ");
	//Serial.print(currentEvent);
	//Serial.print(" n: ");
	//Serial.println(nextState);
	//Serial.println("//----------------------------");

	currentState = nextState;
	//Serial.println("processStateExit");
	//this->printState();
}


void PowercastTxControl3Layer::doTimerActions()
{
	//Serial.println(".");
	unsigned long currentTime;
	//int timeFromLastPing = currentTime - lastPingReceived;
	//int timeForProbeTimeout = currentTime - powerProbeTimer;
	//Serial.println("LAST PING  ---   LAST PROBE TIME");
	//Serial.print(timeFromLastPing);
	//Serial.print("  ---   ");
	//Serial.println(timeForProbeTimeout);

	if (currentState == STATE_OFF_POWER_PROBE) {
		currentTime = getCurrentTimeMiliseconds();
		if (currentTime - powerProbeResponseTimer >= TX4_TIMEOUT_POWER_PROBE_RESPONSE) {
			eventTimeoutPowerProbeResponse();
		}
	}
	
	if (currentState == STATE_ON) {
		currentTime = getCurrentTimeMiliseconds();
		if (currentTime - lastPowerProbeReceived >= TX4_TIMEOUT_TX_PROBE_RECEIVE) {
			eventTimeoutOnPowerProbe();
			lastPowerProbeReceived = currentTime;
		}
	}

	/*
	currentTime = getCurrentTimeMiliseconds();
	Serial.print("---timer actions: ");
	Serial.print(currentTime);
	Serial.print(" -- ");
	Serial.print(powerProbeSentTimer);
	Serial.print(" -- ");
	Serial.print(currentTime - powerProbeSentTimer);
	Serial.println(" -- ");*/

	doMeasurement();
}


void PowercastTxControl3Layer::eventReceivedPing(ProtocolPacket *xBeePacket)
{
	if (measurementObject != NULL) {
		measurementObject->saveFsmEventToFile(IS_TX, EVENT_RECEIVED_PING, currentState);
	}

	processState(EVENT_RECEIVED_PING, xBeePacket);
	
}

void PowercastTxControl3Layer::eventPwrProbeReceivedAboveTh()
{
	//Serial.print(F("currentState "));
	//Serial.print(currentState);
	//Serial.println(F("eventTimeoutPing"));

	if (measurementObject != NULL) {
		measurementObject->saveFsmEventToFile(IS_TX, EVENT_PWR_PROBE_RECEIVED_ABOVE_TH, currentState);
	}
	processState(EVENT_PWR_PROBE_RECEIVED_ABOVE_TH, NULL);
}


void PowercastTxControl3Layer::eventTimeoutPowerProbeResponse()
{
	//Serial.print(F("currentState "));
	//Serial.print(currentState);
	//Serial.println(F("eventTimeoutOnProbe"));
	if (measurementObject != NULL) {
		measurementObject->saveFsmEventToFile(IS_TX, EVENT_TIMEOUT_PWR_PROBE_RESPONSE, currentState);
	}
	processState(EVENT_TIMEOUT_PWR_PROBE_RESPONSE, NULL);
}

void PowercastTxControl3Layer::eventPwrProbeReceivedBelowTh()
{
	//Serial.print(F("currentState "));
	//Serial.print(currentState);
	//Serial.println(F("eventPwrProbeReceivedBelowTh"));
	if (measurementObject != NULL) {
		measurementObject->saveFsmEventToFile(IS_TX, EVENT_PWR_PROBE_RECEIVED_BELOW_TH, currentState);
	}
	processState(EVENT_PWR_PROBE_RECEIVED_BELOW_TH, NULL);
}

void PowercastTxControl3Layer::eventTimeoutOnPowerProbe()
{
	//Serial.print(F("currentState "));
	//Serial.print(currentState);
	//Serial.println(F("eventPwrProbeReceivedBelowTh"));
	if (measurementObject != NULL) {
		measurementObject->saveFsmEventToFile(IS_TX, EVENT_TIMEOUT_ON_POWER_PROBE, currentState);
	}
	processState(EVENT_TIMEOUT_ON_POWER_PROBE, NULL);
}

void PowercastTxControl3Layer::actionTurnOn()
{
	if (measurementObject != NULL) {
		measurementObject->saveFsmActionToFile(IS_TX, ACTION_TURN_ON, currentState);
	}

	makeTxOn(true);
}


void  PowercastTxControl3Layer::actionTurnOff()
{
	if (measurementObject != NULL) {
		measurementObject->saveFsmActionToFile(IS_TX, ACTION_TURN_OFF, currentState);
	}

	makeTxOn(false);
}

void PowercastTxControl3Layer::actionSendPwrProbe(ProtocolPacket *xBeePacket)
{

	if (measurementObject != NULL) {
		measurementObject->saveFsmActionToFile(IS_TX, ACTION_SEND_PWR_PROBE_REQUEST, currentState);
	}

	// random delay before sending to pervent collisions
	randomWaitTimeBeforeSending = random(TX4_RANDOM_WAIT_TIME_MAX);
	delay(randomWaitTimeBeforeSending);

	// Frame format:
	// 1st byte - protocol type
	// 1st byte - packet type
	// 2nd byte - request sequenceNumber
	// remaining bytes - content of request packet
	ProtocolPacket responsePacket;
	responsePacket.protocolType = PROTOCOL_CONTROL;
	responsePacket.packetType = TYPE_VOLTAGE_REQUEST;

	// Frame format:
	// 1st byte - sequence number
	// 2nd and 3rd byte - voltage [mV]
	// 4th and 5th byte - sample number (0-1023)
	// 6th and 7th - voltage reference used [mV]
	byte payload[] = { 0 };
	payload[0] = 11;

	memcpy(responsePacket.packetData, payload, 1);
	responsePacket.packetDataLength = 1;

	if (measurementObject != NULL) {
		measurementObject->savePacketEventToFile(IS_TX, false, &responsePacket);
	}

	if (xBeePacket != NULL) {
		parentLayer->sendPacketToAddress(&responsePacket, xBeePacket->senderAddress);
	}

	powerProbeResponseTimer = getCurrentTimeMiliseconds();

	//Serial.println(powerProbeSentTimer);
	//voltageRequestRepeats++;
}

void PowercastTxControl3Layer::doMeasurement()
{
	if (measurementObject != NULL) {
		measurementObject->updateTransmitterData(digitalRead(pinTxOn), currentState);
	}
}

void PowercastTxControl3Layer::makeTxOn(bool status)
{
	if (status) {
		digitalWrite(pinTxOff, LOW);
		digitalWrite(pinTxOn, HIGH);
	}
	else {
		digitalWrite(pinTxOff, HIGH);
		digitalWrite(pinTxOn, LOW);
	}
}