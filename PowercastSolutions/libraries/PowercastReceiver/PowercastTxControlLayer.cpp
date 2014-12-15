#include "PowercastTxControlLayer.h"


PowercastTxControlLayer::PowercastTxControlLayer()
{
	lastPingReceived = 0;
	currentState = STATE_OFF;
}

void PowercastTxControlLayer::setOutputPins(int pinTxOff, int pinTxOn)
{
	this->pinTxOff = pinTxOff;
	this->pinTxOn = pinTxOn;
}


void PowercastTxControlLayer::processPacket(ProtocolPacket *xBeePacket)
{
	//Serial.println("processPacket");

	if (xBeePacket->packetType == TYPE_PING) {
		//Serial.println("processPacket - type test");

		if (measurementObject != NULL) {
			measurementObject->savePacketEventToFile(IS_TX, true, xBeePacket);
		}

		byte rssiThreshold = xBeePacket->packetData[0];
		if (xBeePacket->receivedPacketRssi < rssiThreshold) {
			eventReceivedPing();
		}

	}
}

void PowercastTxControlLayer::processState(Event currentEvent, ProtocolPacket *xBeePacket)
{
	//Serial.println("processState");

	int nextState = currentState;
	switch (currentState)
	{
	case STATE_OFF:
		if (currentEvent == EVENT_RECEIVED_PING)
		{
			actionTurnOn();
			nextState = STATE_ON;
		}
		break;
	case STATE_ON:
		if (currentEvent == EVENT_TIMEOUT_PING)
		{
			actionTurnOff();
			nextState = STATE_OFF;
		}
		break;
	default:
		break;
	}

	currentState = nextState;

}


void PowercastTxControlLayer::doTimerActions()
{
	unsigned long currentTime = getCurrentTimeMiliseconds();
	if (currentState == STATE_ON && currentTime - lastPingReceived >= TIMEOUT_PING) {
		eventTimeoutPing();
	}

	doMeasurement();
}


void PowercastTxControlLayer::eventReceivedPing()
{
	if (measurementObject != NULL) {
		measurementObject->saveFsmEventToFile(IS_TX, EVENT_RECEIVED_PING, currentState);
	}

	lastPingReceived = getCurrentTimeMiliseconds();
	processState(EVENT_RECEIVED_PING, NULL);
}

void PowercastTxControlLayer::eventTimeoutPing()
{
	if (measurementObject != NULL) {
		measurementObject->saveFsmEventToFile(IS_TX, EVENT_TIMEOUT_PING, currentState);
	}

	processState(EVENT_TIMEOUT_PING, NULL);
}


void PowercastTxControlLayer::actionTurnOn()
{
	if (measurementObject != NULL) {
		measurementObject->saveFsmActionToFile(IS_TX, ACTION_TURN_ON, currentState);
	}

	makeTxOn(true);
}


void  PowercastTxControlLayer::actionTurnOff()
{
	if (measurementObject != NULL) {
		measurementObject->saveFsmActionToFile(IS_TX, ACTION_TURN_OFF, currentState);
	}

	makeTxOn(false);
}


void PowercastTxControlLayer::doMeasurement()
{
	if (measurementObject != NULL) {
		measurementObject->updateTransmitterData(digitalRead(pinTxOn), currentState);
	}
}

void PowercastTxControlLayer::makeTxOn(bool status)
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