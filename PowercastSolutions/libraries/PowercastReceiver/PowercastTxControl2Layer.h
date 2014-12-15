#ifndef PowercastTxControl2Layer_h
#define PowercastTxControl2Layer_h

#include "PowercastLayer.h"

#define IS_TX true

#define TYPE_PING 0
#define TYPE_TEST 1
#define TYPE_TEST_RESPONSE 2
#define TYPE_VOLTAGE_REQUEST 3
#define TYPE_VOLTAGE_RESPONSE 4

#define STATE_OFF 0
#define STATE_ON 1
#define STATE_OFF_POWER_PROBE 2
#define STATE_ON_POWER_PROBE 3

#define EVENT_RECEIVED_PING 0
#define EVENT_TIMEOUT_PING 1
#define EVENT_PWR_PROBE_RECEIVED_ABOVE_TH 2
#define EVENT_PWR_PROBE_RECEIVED_BELOW_TH 3
#define EVENT_TIMEOUT_ON_PROBE 4
#define EVENT_PWR_PROBE_TIMEOUT 5

#define ACTION_TURN_ON 0
#define ACTION_TURN_OFF 1
#define ACTION_SEND_PWR_PROBE 2

#include "PowercastControlParameters.h"

class PowercastTxControl2Layer: public PowercastLayer
{
public:
	PowercastTxControl2Layer();
	void setOutputPins(int pinTxOff, int pinTxOn);

	void processPacket(ProtocolPacket *xBeePacket);
	void doTimerActions();
	void processState(Event currentEvent, ProtocolPacket *xBeePacket);

	//EVENTS
	void eventReceivedPing();
	void eventTimeoutPing();
	void eventPwrProbeReceivedAboveTh();
	void eventPwrProbeReceivedBelowTh();
	void eventTimeoutOnProbe();
	void eventPwrProbeTimeout();

	// ACTIONS
	void actionTurnOn();
	void actionTurnOff();
	void actionSendPwrProbe();

	void doMeasurement();

private:
	void makeTxOn(bool status);

	unsigned long lastPingReceived;
	unsigned long powerProbeTimer;
	unsigned long powerProbeSentTimer;
	int voltageRequestRepeats;

	int pinTxOff;
	int pinTxOn;
};

#endif