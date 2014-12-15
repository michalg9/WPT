/*
  PowercastComm.h - Library for Powercast Wireless Power Network Communication.
  Created by Michal Golinski, April 23, 2014.
*/

#ifndef PowercastComm_h
#define PowercastComm_h

#include "Arduino.h"
#include <XBee.h>

#define ADDRESS_BROADCAST 0xffff
#define ADDRESS_PC 0x3333
#define TYPE_PING 0
#define TYPE_VOLTAGE_REPORT 1
#define TYPE_RSSI_REPORT 2
#define TYPE_VOLTAGE_REQUEST 3
#define TYPE_RSSI_REQUEST 4
#define TYPE_RSSI_PING 5
#define TYPE_TEST 6
#define TYPE_TEST_RESPONSE 7
#define TYPE_CHANGE_REFERENCE 8
#define TYPE_CHANGE_REFERENCE_RESPONSE 9
#define TYPE_START_RSSI_PING 10
#define TYPE_STOP_RSSI_PING 11
#define TYPE_SET_RSSI_THRESHOLD_REQUEST 12
#define TYPE_SET_RSSI_THRESHOLD_RESPONSE 13
#define TYPE_TIME_SYNCH 14
#define highByte(number) ((number >> 8) & 0xff)
#define lowByte(number) (number & 0xff)
#define makeInteger(hByte, lByte) (hByte << 8 | lByte)


#define PING_THRESHOLD 2000
#define RSSI_OFF_THRESHOLD 55
#define RSSI_ON_THRESHOLD 55

typedef struct
  {
    int referenceValMv;
    int calculatedVoltageMv;
    int usedSampleValue;
  } voltageMeasurementStruct;
  

enum PowercastState {
	Off,
	On
};

enum PowercastEvent {
	PingReceived,
	PingReceivedTimeout
};

class PowercastComm
{
public:
	PowercastComm();
    void setSerial(Stream &serial);
	void setReferencePointers(int* internalReferenceValMvArduino, int* defaultReferenceValMvArduino);
    boolean tryReceivePacket();
	//void processPacket();
	void processPacket(voltageMeasurementStruct* voltageMeasurementStructData);
	void sendVoltageReport(byte sequenceNumber, voltageMeasurementStruct* voltageMeasurementStructData);
	void sendRssiStartPing(byte sequenceNumber, int numberOfValues);
	void sendRssiStopPing(byte sequenceNumber);
	void sendRssiPing(byte sequenceNumber);
	void sendPing();
	void sendRssiReport(byte sequenceNumber, byte rssiValue);
	void sendRssiReport(byte sequenceNumber, byte rssiValues[], int numOfValues);
	void sendTestResponse(byte sequenceNumber);
	void broadcastTimeSynch();
	void sendChangeReferenceResponse(byte sequenceNumber);
	void changeReference(int internalReferenceValue, int defaultReferenceValue);
	void setOutputPins(int pinTxOff, int pinTxOn);
	void checkForPing();
	bool getTxStatus();

	void evaluateState(PowercastState currentState, PowercastEvent currentEvent);
	void applyState(PowercastState currentState);

	unsigned long recentReceivedTimestamp;
	unsigned long localTimestampOnSynch;
private:
	void makeTxOn(bool status);
	void convertLongToByteArray(long longInt, byte byteArray[]);
	long convertByteArrayToLong(byte byteArray[]);
	
	XBee* xbee;
	Rx16Response rx16;
	TxStatusResponse txStatusResponse;
	
	int* internalReferenceValMv;
	int* defaultReferenceValMv;
	
	int pinTxOff;
	int pinTxOn;
	unsigned long lastPingReceived;
	bool txStatus;
	
	byte rssiThreshold;

	PowercastState powercastState;

};

#endif