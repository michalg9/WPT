#ifndef PowercastRxMeasurementLayer_h
#define PowercastRxMeasurementLayer_h

#include "PowercastLayer.h"
#include "MeasurementObject.h"

#define TYPE_TIME_SYNCH 0
#define TYPE_SET_COMM_THRESHOLD 1
#define TYPE_SET_POWER_THRESHOLD 1

#define STATE_IDLE 0

#define EVENT_TIMEOUT_MEASUREMENT 0
#define EVENT_TIMEOUT_TIME_SYNCH 1

#define DELAY_TIME_SYNCH 4000U //4294967294U //30000U
#define DELAY_MEASUREMENT 100U

class PowercastRxMeasurementLayer: public PowercastLayer
{
public:
	PowercastRxMeasurementLayer();
	void setMeasurementObject(MeasurementObject *measObject);

	void processPacket(ProtocolPacket *xBeePacket);
	void doTimerActions();
	void processState(Event currentEvent, ProtocolPacket *xBeePacket);

	//EVENTS
	void eventTimeoutTimeSynch();
	void eventTimeoutMeasurement();

	// ACTIONS
	void actionSendTimeSynchBroadcast();
	void actionSaveResultsToSd();

private:
	MeasurementObject *measurementObject;

	unsigned long timerMeasurement;

	void convertLongToByteArray(long longInt, byte byteArray[]);
	long convertByteArrayToLong(byte byteArray[]);

	unsigned long timerTimeSynch;
	bool firstTimeSynchSent;
};

#endif