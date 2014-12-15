#ifndef PowercastTxMeasurementLayer_h
#define PowercastTxMeasurementLayer_h

#include "PowercastLayer.h"
#include "MeasurementObject.h"

#define TYPE_TIME_SYNCH 0

#define STATE_IDLE 0

#define EVENT_TIMEOUT_MEASUREMENT 0
#define EVENT_RECEIVED_TIME_SYNCH 1

#define DELAY_MEASUREMENT 100U

class PowercastTxMeasurementLayer: public PowercastLayer
{
public:
	PowercastTxMeasurementLayer();
	void setMeasurementObject(MeasurementObject *measObject);

	void processPacket(ProtocolPacket *xBeePacket);
	void doTimerActions();
	void processState(Event currentEvent, ProtocolPacket *xBeePacket);

	//EVENTS
	void eventReceivedTimeSynch(ProtocolPacket *xBeePacket);
	void eventTimeoutMeasurement();

	// ACTIONS
	void actionSaveTimeSynch(ProtocolPacket *xBeePacket);
	void actionSaveResultsToSd();

private:
	MeasurementObject *measurementObject;

	unsigned long timerMeasurement;

	//void convertLongToByteArray(long longInt, byte byteArray[]);
	long convertByteArrayToLong(byte byteArray[]);
};

#endif