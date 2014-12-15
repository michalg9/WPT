#include "PowercastRxControlLayer.h"


PowercastRxControlLayer::PowercastRxControlLayer()
{
	timerPing = 0;
	currentState = STATE_IDLE;
}

void PowercastRxControlLayer::resetMeasurement()
{

}

void PowercastRxControlLayer::processPacket(ProtocolPacket *xBeePacket)
{

}

void PowercastRxControlLayer::eventTimeoutPing()
{
	if (measurementObject != NULL) {
		measurementObject->saveFsmEventToFile(IS_TX, EVENT_TIMEOUT_PING, currentState);
	}
	processState(EVENT_TIMEOUT_PING, NULL);
}

void PowercastRxControlLayer::processState(Event currentEvent, ProtocolPacket *xBeePacket)
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
		nextState = currentState;
		break;
	default:
		break;
	}
}


void PowercastRxControlLayer::doTimerActions()
{
	unsigned long currentTime = getCurrentTimeMiliseconds();
	if (currentTime - timerPing >= DELAY_PING) {
		eventTimeoutPing();
		timerPing = currentTime;
	}

	doMeasurement();
}

void  PowercastRxControlLayer::actionSendPing()
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
	byte payload[] = { RSSI_THRESHOLD };
	memcpy(responsePacket.packetData, payload, 1);
	responsePacket.packetDataLength = 1;

	// old version of ping
	//responsePacket.packetData = NULL;
	//responsePacket.packetDataLength = 0;

	parentLayer->sendPacketBroadcast(&responsePacket);
	
}

void PowercastRxControlLayer::doMeasurement()
{
	if (measurementObject != NULL) {
		measurementObject->updateReceiverData(voltageMeasurementStructData->calculatedVoltageMv);
	}
}
