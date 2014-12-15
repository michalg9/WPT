#include "PowercastLayer.h"


PowercastLayer::PowercastLayer()
{
	parentLayer = NULL;
	measurementObject = NULL;
}

void PowercastLayer::setParentLayer(PowercastReceiver* parent)
{
	parentLayer = parent;
}

void PowercastLayer::setMeasurementObject(MeasurementObject *measObject)
{
	measurementObject = measObject;
}

bool PowercastLayer::isTheAddressAllowed(uint16_t address)
{
	return true;
}

unsigned long PowercastLayer::getCurrentTimeMiliseconds()
{
	return millis();
}