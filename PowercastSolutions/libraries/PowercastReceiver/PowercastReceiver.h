#ifndef PowercastReceiver_h
#define PowercastReceiver_h

#include "Arduino.h"
#include <XBee.h>
#include "PowercastLayer.h"
#include "MeasurementObject.h"

#define PROTOCOL_CONTROL 0
#define PROTOCOL_TIMESYNCH 1

class PowercastLayer;

#define ADDRESS_BROADCAST 0xFFFF
#define MAX_PACKET_DATA_LENGTH 10


class ProtocolPacket {
public:
	byte protocolType;
	byte packetType;
	byte packetData[MAX_PACKET_DATA_LENGTH];
	int packetDataLength;
	byte receivedPacketRssi;

	uint16_t senderAddress;

	ProtocolPacket()
	{

	}

	
	int getPacketDataSize() {
		return sizeof(byte)*getPacketSize();
	}

	byte* getBytes() {
		bytesToSend[0] = protocolType;
		bytesToSend[1] = packetType;
		for (int i = 0; i < getPacketSize(); i++)
		{
			bytesToSend[2 + i] = packetData[i];
		}

		return bytesToSend;
	}

private:
	byte bytesToSend[MAX_PACKET_DATA_LENGTH + 2];

	int getPacketSize() {
		// protocolType + packetType (2) + packetDataLength
		return (2 + packetDataLength);
	}

};

class PowercastReceiver
{
public:
	PowercastReceiver();
	void setSerial(Stream &serial);
	void setControlLayer(PowercastLayer* powercastLayer);
	void setMeasurementLayer(PowercastLayer* powercastLayer);
	void setMeasurementObject(MeasurementObject *measObject);

	boolean tryReceivePacket();
	ProtocolPacket* obtainPacket();

	void executeProtocol();

	void sendPacketToAddress(ProtocolPacket* packetToSend, uint16_t address);
	void sendPacketBroadcast(ProtocolPacket* packetToSend);

	bool enableSending;

private:
	XBee* xbee;
	Rx16Response rx16;
	TxStatusResponse txStatusResponse;

	PowercastLayer* powercastControlLayer;
	PowercastLayer* powercastMeasurementLayer;
	MeasurementObject *measurementObject;

	ProtocolPacket receivePacket;

};

#endif