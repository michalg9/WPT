#ifndef PowercastLayer_h
#define PowercastLayer_h

#define highByte(number) ((number >> 8) & 0xff)
#define lowByte(number) (number & 0xff)
#define makeInteger(hByte, lByte) (hByte << 8 | lByte)

#include "PowercastReceiver.h"
#include "MeasurementObject.h"

class PowercastReceiver;
class ProtocolPacket;
class MeasurementObject;

typedef int State;
typedef int Event;

class PowercastLayer
{

public:
	PowercastLayer();
	void setParentLayer(PowercastReceiver* parent);

	void setMeasurementObject(MeasurementObject *measObject);

	virtual void processPacket(ProtocolPacket *xBeePacket) = 0;
	virtual void doTimerActions() = 0;
	virtual void processState(Event currentEvent, ProtocolPacket *xBeePacket) = 0;

	bool isTheAddressAllowed(uint16_t address);

	//EVENTS
	// implement your event functions here

	// ACTIONS
	// implement your action functions here

protected:
	PowercastReceiver* parentLayer;
	State currentState;

	MeasurementObject *measurementObject;

	unsigned long getCurrentTimeMiliseconds();
};


#endif