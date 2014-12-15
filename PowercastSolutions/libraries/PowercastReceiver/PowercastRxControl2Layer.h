#ifndef PowercastRxControl2Layer_h
#define PowercastRxControl2Layer_h

#include "PowercastLayer.h"

#define IS_TX false

#define TYPE_PING 0
#define TYPE_TEST 1
#define TYPE_TEST_RESPONSE 2
#define TYPE_VOLTAGE_REQUEST 3
#define TYPE_VOLTAGE_RESPONSE 4

#define STATE_IDLE 0

#define EVENT_TIMEOUT_PING 0
#define EVENT_TEST_REQUEST_RECEIVED 1
#define EVENT_VOLTAGE_REQUEST_RECEIVED 2

#define DELAY_PING 1000U

typedef struct
{
	int referenceValMv;
	int calculatedVoltageMv;
	int usedSampleValue;
} voltageMeasurementStruct;

class PowercastRxControl2Layer: public PowercastLayer
{

public:
	PowercastRxControl2Layer();
	
	void processPacket(ProtocolPacket *xBeePacket);
	void doTimerActions();
	void processState(Event currentEvent, ProtocolPacket *xBeePacket);

	//EVENTS
	void eventReceivedTestPacket(ProtocolPacket *xBeePacket);
	void eventTimeoutPing();
	void eventReceivedVoltagePacket(ProtocolPacket *xBeePacket);

	// ACTIONS
	void actionSendTestResponse(ProtocolPacket *xBeePacket);
	void actionSendPing();
	void actionSendVoltageResponse(ProtocolPacket *xBeePacket);

	voltageMeasurementStruct* voltageMeasurementStructData;

	void doMeasurement();

private:
	unsigned long timerPing;
};

#endif