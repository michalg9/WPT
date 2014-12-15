#include "PowercastTxControl2Layer.h"


PowercastTxControl2Layer::PowercastTxControl2Layer()
{
	lastPingReceived = 0;
	powerProbeTimer = 0;
	powerProbeSentTimer = 0;
	voltageRequestRepeats = 0;
	currentState = STATE_OFF;
	measurementObject = NULL;
}

void PowercastTxControl2Layer::setOutputPins(int pinTxOff, int pinTxOn)
{
	this->pinTxOff = pinTxOff;
	this->pinTxOn = pinTxOn;
}

void PowercastTxControl2Layer::processPacket(ProtocolPacket *xBeePacket)
{
	//Serial.println("processPacket");

	if (xBeePacket->packetType == TYPE_PING) {

		if (measurementObject != NULL) {
			measurementObject->savePacketEventToFile(IS_TX, true, xBeePacket);
		}

		if (xBeePacket->receivedPacketRssi < RSSI_THRESHOLD) {
			eventReceivedPing();
		}
	}
	else if (xBeePacket->packetType == TYPE_VOLTAGE_RESPONSE) {

		if (measurementObject != NULL) {
			measurementObject->savePacketEventToFile(IS_TX, true, xBeePacket);
		}

		int receivedVoltage = makeInteger(xBeePacket->packetData[1], xBeePacket->packetData[2]);

		if (receivedVoltage > VOLTAGE_THRESHOLD) {
			eventPwrProbeReceivedAboveTh();
		}
		else {
			eventPwrProbeReceivedBelowTh();
		}
	}
}

void PowercastTxControl2Layer::processState(Event currentEvent, ProtocolPacket *xBeePacket)
{
	//Serial.println("processStateEnter");
	//this->printState();
	int nextState = currentState;
	switch (currentState)
	{
	case STATE_OFF:
		if (currentEvent == EVENT_RECEIVED_PING)
		{
			actionSendPwrProbe();
			nextState = STATE_OFF_POWER_PROBE;
		}
		break;
	case STATE_OFF_POWER_PROBE:
		if (currentEvent == EVENT_PWR_PROBE_RECEIVED_ABOVE_TH)
		{
			actionTurnOff();
			nextState = STATE_OFF;
		}
		else if (currentEvent == EVENT_PWR_PROBE_RECEIVED_BELOW_TH)
		{
			actionTurnOn();
			powerProbeTimer = getCurrentTimeMiliseconds() - (TIMEOUT_ON_PROBE - FIRST_CHECK_AFTER);
			nextState = STATE_ON;
		}
		if (currentEvent == EVENT_TIMEOUT_PING)
		{
			actionTurnOff();
			nextState = STATE_OFF;
		}
		if (currentEvent == EVENT_PWR_PROBE_TIMEOUT)
		{
			actionTurnOff();
			nextState = STATE_OFF;
		}
		break;
	case STATE_ON:
		if (currentEvent == EVENT_TIMEOUT_PING)
		{
			actionTurnOff();
			nextState = STATE_OFF;
		}
		else if (currentEvent == EVENT_TIMEOUT_ON_PROBE)
		{
			actionSendPwrProbe();
			nextState = STATE_ON_POWER_PROBE;
		}
		break;
	case STATE_ON_POWER_PROBE:
		if (currentEvent == EVENT_PWR_PROBE_RECEIVED_ABOVE_TH)
		{
			actionTurnOn();
			nextState = STATE_ON;
		}
		else if (currentEvent == EVENT_PWR_PROBE_RECEIVED_BELOW_TH)
		{
			actionTurnOff();
			nextState = STATE_OFF;
		}
		if (currentEvent == EVENT_TIMEOUT_PING)
		{
			actionTurnOff();
			nextState = STATE_OFF;
		}
		if (currentEvent == EVENT_PWR_PROBE_TIMEOUT)
		{
			actionTurnOff();
			nextState = STATE_OFF;
		}
		break;
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


void PowercastTxControl2Layer::doTimerActions()
{
	//Serial.println(".");
	unsigned long currentTime;
	//int timeFromLastPing = currentTime - lastPingReceived;
	//int timeForProbeTimeout = currentTime - powerProbeTimer;
	//Serial.println("LAST PING  ---   LAST PROBE TIME");
	//Serial.print(timeFromLastPing);
	//Serial.print("  ---   ");
	//Serial.println(timeForProbeTimeout);

	currentTime = getCurrentTimeMiliseconds();
	if ((currentState == STATE_ON || currentState == STATE_ON_POWER_PROBE || currentState == STATE_OFF_POWER_PROBE) && currentTime - lastPingReceived >= TIMEOUT_PING) {
		eventTimeoutPing();
	}

	currentTime = getCurrentTimeMiliseconds();
	if (currentState == STATE_ON && currentTime - powerProbeTimer >= TIMEOUT_ON_PROBE) {
		eventTimeoutOnProbe();
		powerProbeTimer = currentTime;
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

	currentTime = getCurrentTimeMiliseconds();
	if ((currentState == STATE_OFF_POWER_PROBE || currentState == STATE_ON_POWER_PROBE) && (currentTime - powerProbeSentTimer >= TIMEOUT_PROBE_SENT)) {
		eventPwrProbeTimeout();
	}

	doMeasurement();
}


void PowercastTxControl2Layer::eventReceivedPing()
{
	if (measurementObject != NULL) {
		measurementObject->saveFsmEventToFile(IS_TX, EVENT_RECEIVED_PING, currentState);
	}

	lastPingReceived = getCurrentTimeMiliseconds();
	processState(EVENT_RECEIVED_PING, NULL);
	
}

void PowercastTxControl2Layer::eventTimeoutPing()
{
	//Serial.print(F("currentState "));
	//Serial.print(currentState);
	//Serial.println(F("eventTimeoutPing"));

	if (measurementObject != NULL) {
		measurementObject->saveFsmEventToFile(IS_TX, EVENT_TIMEOUT_PING, currentState);
	}
	processState(EVENT_TIMEOUT_PING, NULL);
}


void PowercastTxControl2Layer::eventPwrProbeReceivedAboveTh()
{
	//Serial.print(F("currentState "));
	//Serial.print(currentState);
	//Serial.println(F(" eventPwrProbeReceivedAboveTh"));

	if (measurementObject != NULL) {
		measurementObject->saveFsmEventToFile(IS_TX, EVENT_PWR_PROBE_RECEIVED_ABOVE_TH, currentState);
	}
	processState(EVENT_PWR_PROBE_RECEIVED_ABOVE_TH, NULL);
}

void PowercastTxControl2Layer::eventPwrProbeReceivedBelowTh()
{
	//Serial.print(F("currentState "));
	//Serial.print(currentState);
	//Serial.println(F("eventPwrProbeReceivedBelowTh"));
	if (measurementObject != NULL) {
		measurementObject->saveFsmEventToFile(IS_TX, EVENT_PWR_PROBE_RECEIVED_BELOW_TH, currentState);
	}
	processState(EVENT_PWR_PROBE_RECEIVED_BELOW_TH, NULL);
}

void PowercastTxControl2Layer::eventTimeoutOnProbe()
{
	//Serial.print(F("currentState "));
	//Serial.print(currentState);
	//Serial.println(F("eventTimeoutOnProbe"));
	if (measurementObject != NULL) {
		measurementObject->saveFsmEventToFile(IS_TX, EVENT_TIMEOUT_ON_PROBE, currentState);
	}
	processState(EVENT_TIMEOUT_ON_PROBE, NULL);
}

void PowercastTxControl2Layer::eventPwrProbeTimeout()
{
	//Serial.print(F("currentState "));
	//Serial.print(currentState);
	//Serial.println(F("eventPwrProbeTimeout"));
	if (measurementObject != NULL) {
		measurementObject->saveFsmEventToFile(IS_TX, EVENT_PWR_PROBE_TIMEOUT, currentState);
	}
	processState(EVENT_PWR_PROBE_TIMEOUT, NULL);
}

void PowercastTxControl2Layer::actionTurnOn()
{
	if (measurementObject != NULL) {
		measurementObject->saveFsmActionToFile(IS_TX, ACTION_TURN_ON, currentState);
	}

	makeTxOn(true);
}


void  PowercastTxControl2Layer::actionTurnOff()
{
	if (measurementObject != NULL) {
		measurementObject->saveFsmActionToFile(IS_TX, ACTION_TURN_OFF, currentState);
	}

	makeTxOn(false);
}

void PowercastTxControl2Layer::actionSendPwrProbe()
{
	if (measurementObject != NULL) {
		measurementObject->saveFsmActionToFile(IS_TX, ACTION_SEND_PWR_PROBE, currentState);
	}

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

	parentLayer->sendPacketToAddress(&responsePacket, ADDRESS_BROADCAST);

	powerProbeSentTimer = getCurrentTimeMiliseconds();
	//Serial.println(powerProbeSentTimer);
	//voltageRequestRepeats++;
}

void PowercastTxControl2Layer::doMeasurement()
{
	if (measurementObject != NULL) {
		measurementObject->updateTransmitterData(digitalRead(pinTxOn), currentState);
	}
}

void PowercastTxControl2Layer::makeTxOn(bool status)
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