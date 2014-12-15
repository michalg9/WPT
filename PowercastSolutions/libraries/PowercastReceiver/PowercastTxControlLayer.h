#ifndef PowercastTxControlLayer_h
#define PowercastTxControlLayer_h

#include "PowercastLayer.h"

#define IS_TX true

#define TYPE_PING 0

#define STATE_OFF 0
#define STATE_ON 1

#define EVENT_RECEIVED_PING 0
#define EVENT_TIMEOUT_PING 1

#define ACTION_TURN_ON 0
#define ACTION_TURN_OFF 1

#include "PowercastControlParameters.h"

class PowercastTxControlLayer: public PowercastLayer
{
public:
	PowercastTxControlLayer();
	void setOutputPins(int pinTxOff, int pinTxOn);

	void processPacket(ProtocolPacket *xBeePacket);
	void doTimerActions();
	void processState(Event currentEvent, ProtocolPacket *xBeePacket);

	//EVENTS
	void eventReceivedPing();
	void eventTimeoutPing();

	// ACTIONS
	void actionTurnOn();
	void actionTurnOff();

	void doMeasurement();

private:
	void makeTxOn(bool status);

	unsigned long lastPingReceived;

	int pinTxOff;
	int pinTxOn;
};

#endif