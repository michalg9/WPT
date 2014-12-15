#ifndef MeasurementObject_h
#define MeasurementObject_h

#include "PowercastReceiver.h"
#include "PowercastControlParameters.h"

class ProtocolPacket;

#define LOG_FILENAME_PREFIX "MEAS"
#define LOG_EVENT_FILENAME_PREFIX "EVENT"
#include "Arduino.h"

#include <SD.h>
const int sdChipSelect = 4;


typedef struct {
	int isTurnedOn;
	int fsmState;
} transmitterMeasurementStruct;

typedef struct {
	int receivedPower;
} receiverMeasurementStruct;

class MeasurementObject
{
public:
	MeasurementObject();
	
	void initializeObject();
	void createNewFile();
	void createNewEventFile();
	void saveFileHeaders();

	void updateTransmitterData(int isTurnedOn, int fsmState);
	void saveTransmitterDataToFile();

	void updateReceiverData(int receivedPower);
	void saveReceiverDataToFile();
	void savePacketEventToFile(bool isTx, bool isReceiveEvent, ProtocolPacket *xBeePacket);
	void savePacketAckToFile(bool isSuccess);

	void saveFsmEventToFile(bool isTx, int eventCode, int state);
	void saveFsmActionToFile(bool isTx, int actionCode, int state);
	void saveButtonEventToFile(bool isRegularEvent);
	void saveStringToSd(String &stringToSave, bool isEvent);


	unsigned long recentReceivedTimestamp;
	unsigned long localTimestampOnSynch;

	char filenameArray[13];
	//char filenameLogArray[13];

	transmitterMeasurementStruct txMeasurementStruct;
	receiverMeasurementStruct rxMeasurementStruct;

	File* dataFile;
	File dataFileLog;
	File dataFileEventLog;

	String dataString;
};



#endif