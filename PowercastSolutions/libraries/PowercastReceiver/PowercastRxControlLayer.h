#ifndef PowercastRxControlLayer_h
#define PowercastRxControlLayer_h

#include "PowercastLayer.h"

#define IS_TX false

#define TYPE_PING 0

#define STATE_IDLE 0

#define EVENT_TIMEOUT_PING 0

#define ACTION_SEND_PING 0

#include "PowercastControlParameters.h"

typedef struct
{
	int referenceValMv;
	int calculatedVoltageMv;
	int usedSampleValue;
} voltageMeasurementStruct;

class PowercastRxControlLayer: public PowercastLayer
{
public:
	PowercastRxControlLayer();
	void resetMeasurement();

	void processPacket(ProtocolPacket *xBeePacket);
	void doTimerActions();
	void processState(Event currentEvent, ProtocolPacket *xBeePacket);

	//EVENTS
	void eventTimeoutPing();

	// ACTIONS
	void actionSendPing();

	voltageMeasurementStruct* voltageMeasurementStructData;

	void doMeasurement();

private:
	unsigned long timerPing;
};

#endif