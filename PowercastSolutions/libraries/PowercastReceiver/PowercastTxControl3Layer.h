#ifndef PowercastTxControl3Layer_h
#define PowercastTxControl3Layer_h

#include "PowercastLayer.h"

#define IS_TX true

#define TYPE_PING 0
#define TYPE_VOLTAGE_REQUEST 1
#define TYPE_VOLTAGE_RESPONSE 2

#define STATE_OFF 0
#define STATE_OFF_POWER_PROBE 1
#define STATE_ON 2

#define EVENT_RECEIVED_PING 0
#define EVENT_PWR_PROBE_RECEIVED_ABOVE_TH 1
#define EVENT_TIMEOUT_PWR_PROBE_RESPONSE 2
#define EVENT_PWR_PROBE_RECEIVED_BELOW_TH 3
#define EVENT_TIMEOUT_ON_POWER_PROBE 4

#define ACTION_SEND_PWR_PROBE_REQUEST 0
#define ACTION_TURN_OFF 1
#define ACTION_TURN_ON 2

#include "PowercastControlParameters.h"

class PowercastTxControl3Layer: public PowercastLayer
{
public:
	PowercastTxControl3Layer();
	void setOutputPins(int pinTxOff, int pinTxOn);

	void processPacket(ProtocolPacket *xBeePacket);
	void doTimerActions();
	void processState(Event currentEvent, ProtocolPacket *xBeePacket);


	//EVENTS
	void eventReceivedPing(ProtocolPacket *xBeePacket);
	void eventPwrProbeReceivedAboveTh();
	void eventTimeoutPowerProbeResponse();
	void eventPwrProbeReceivedBelowTh();
	void eventTimeoutOnPowerProbe();

	// ACTIONS
	void actionSendPwrProbe(ProtocolPacket *xBeePacket);
	void actionTurnOff();
	void actionTurnOn();

	void doMeasurement();

private:
	void makeTxOn(bool status);

	unsigned long lastPowerProbeReceived;
	unsigned long powerProbeResponseTimer;
	long randomWaitTimeBeforeSending;

	int pinTxOff;
	int pinTxOn;
};

#endif