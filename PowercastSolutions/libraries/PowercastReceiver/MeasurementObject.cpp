#include "MeasurementObject.h"

MeasurementObject::MeasurementObject()
{
	recentReceivedTimestamp = 0;
	localTimestampOnSynch = 0;
	dataString.reserve(50);
}

void MeasurementObject::initializeObject()
{
#ifdef __SD_H__
	pinMode(10, OUTPUT);
	if (!SD.begin(sdChipSelect)) {
		//Serial.println(F("Card failed, or not present"));
		// don't do anything more:
		return;
	}
	createNewFile();
	createNewEventFile();
	saveFileHeaders();
#endif
}

void MeasurementObject::createNewFile()
{
#ifdef __SD_H__
	for (int i = 1; i < 10; i++) {
		dataString = "";
		dataString += LOG_FILENAME_PREFIX;
		dataString += i;
		dataString += ".txt";
		dataString.toCharArray(filenameArray, 13);
		if (!SD.exists(filenameArray)) {
			break;
		}
	}

	dataFileLog = SD.open(filenameArray, FILE_WRITE);

	//Serial.print(F("New filename: "));
	//Serial.println(filenameArray);
#endif
}

void MeasurementObject::createNewEventFile()
{
#ifdef __SD_H__
	for (int i = 1; i < 10; i++) {
		dataString = "";
		dataString += LOG_EVENT_FILENAME_PREFIX;
		dataString += i;
		dataString += ".txt";
		dataString.toCharArray(filenameArray, 13);
		if (!SD.exists(filenameArray)) {
			break;
		}
	}

	dataFileEventLog = SD.open(filenameArray, FILE_WRITE);

	//Serial.print(F("New filename: "));
	//Serial.println(filenameLogArray);
#endif
}

void MeasurementObject::saveFileHeaders() {
	dataString = "";
	dataString += "p:";
	dataString += String(DELAY_PING);
	dataString += ",";
	dataString += String(DELAY_REMOVE_LAST_PROBE_SENDER);
	dataString += ",";
	dataString += String(DELAY_WAITING_FOR_SECOND_PROBE);
	dataString += ",";
	dataString += String(VOLTAGE_THRESHOLD);
	dataString += ",";
	dataString += String(TIMEOUT_PING);
	dataString += ",";
	dataString += String(TIMEOUT_ON_PROBE);
	dataString += ",";
	dataString += String(FIRST_CHECK_AFTER);
	dataString += ",";
	dataString += String(TIMEOUT_PROBE_SENT);
	dataString += ",";
	dataString += String(RSSI_THRESHOLD);
	saveStringToSd(dataString, false);
	saveStringToSd(dataString, true);
}

void MeasurementObject::updateTransmitterData(int isTurnedOn, int fsmState)
{
	txMeasurementStruct.isTurnedOn = isTurnedOn;
	txMeasurementStruct.fsmState = fsmState;
}

void MeasurementObject::saveTransmitterDataToFile()
{
	if (recentReceivedTimestamp != 0) {
		// make a string for assembling the data to log:
		dataString = "";
		dataString += String(millis() - localTimestampOnSynch + recentReceivedTimestamp);
		dataString += ",";
		dataString += String(txMeasurementStruct.isTurnedOn);
		dataString += ",";
		dataString += String(txMeasurementStruct.fsmState);

		saveStringToSd(dataString, false);
	}
}

void MeasurementObject::updateReceiverData(int receivedPower)
{
	rxMeasurementStruct.receivedPower = receivedPower;
}

void MeasurementObject::saveReceiverDataToFile()
{
		// make a string for assembling the data to log:
		dataString = "";
		dataString += String(millis());
		dataString += ",";
		dataString += String(rxMeasurementStruct.receivedPower);

		saveStringToSd(dataString, false);
}

void MeasurementObject::savePacketEventToFile(bool isTx, bool isReceiveEvent, ProtocolPacket *xBeePacket)
{
		// make a string for assembling the data to log:
		dataString = "";
		if (isTx) {
			dataString += String(millis() - localTimestampOnSynch + recentReceivedTimestamp);
		}
		else {
			dataString += String(millis());
		}
		dataString += ",packet,";
		dataString += String(isReceiveEvent);
		dataString += ",";
		dataString += String(xBeePacket->protocolType);
		dataString += ",";
		dataString += String(xBeePacket->packetType);
		dataString += ",";
		dataString += String(xBeePacket->packetDataLength);
		if (isReceiveEvent)
		{
			dataString += ",";
			dataString += String(xBeePacket->receivedPacketRssi);
			dataString += ",";
			dataString += String(xBeePacket->senderAddress, HEX);
		}	
		saveStringToSd(dataString, true);
}

void MeasurementObject::savePacketAckToFile(bool isSuccess)
{
	// make a string for assembling the data to log:
	dataString = "";
	dataString += String(millis() - localTimestampOnSynch + recentReceivedTimestamp);
	dataString += ",packetAck,";
	dataString += String(isSuccess);
	saveStringToSd(dataString, true);
}


void MeasurementObject::saveFsmEventToFile(bool isTx, int eventCode, int state)
{
		// make a string for assembling the data to log:
		dataString = "";
		if (isTx) {
			dataString += String(millis() - localTimestampOnSynch + recentReceivedTimestamp);
		}
		else {
			dataString += String(millis());
		}
		dataString += ",event,";
		dataString += String(eventCode);
		dataString += ",";
		dataString += String(state);
		saveStringToSd(dataString, true);
}

void MeasurementObject::saveFsmActionToFile(bool isTx, int actionCode, int state)
{
	// make a string for assembling the data to log:
	dataString = "";
	if (isTx) {
		dataString += String(millis() - localTimestampOnSynch + recentReceivedTimestamp);
	}
	else {
		dataString += String(millis());
	}
	dataString += ",action,";
	dataString += String(actionCode);
	dataString += ",";
	dataString += String(state);
	saveStringToSd(dataString, true);
}

void MeasurementObject::saveButtonEventToFile(bool isRegularEvent)
{
	// make a string for assembling the data to log:
	dataString = "";
	dataString += String(millis());
	if (isRegularEvent)
	{
		dataString += ",buttonPress";
	}
	else
	{
		dataString += ",buttonEnd";
	}
	saveStringToSd(dataString, true);
}


void MeasurementObject::saveStringToSd(String &stringToSave, bool isEvent)
{
#ifdef __SD_H__
		// open the file. note that only one file can be open at a time,
		// so you have to close this one before opening another.
	
	//Serial.print(" freeMemory()= ");
	//Serial.println(freeMemory());
	//while (!dataFile)
	//{
		//Serial.print(F("Trying to open "));
		if (isEvent)
		{
			//dataFile = SD.open(filenameLogArray, FILE_WRITE);
			dataFile = &dataFileEventLog;
			//Serial.println(filenameLogArray);
			//Serial.println("----------------EVENT");
		}
		else
		{
			//dataFile = SD.open(filenameArray, FILE_WRITE);
			dataFile = &dataFileLog;
			//Serial.println(filenameArray);
		}
	//}
	//Serial.print(" freeMemory2()= ");
	//Serial.println(freeMemory());
	//Serial.print(F("LOGGING FILE "));
	//Serial.print(dataFile);
	//Serial.print(F(" - "));
	//dataFile = SD.open(filenameLogArray, FILE_WRITE);
		// if the file is available, write to it:
	if (dataFile)
	{
		dataFile->println(stringToSave);
		dataFile->flush();
		
		//dataFile.close();
	
		// print to the serial port too:
		//Serial.print(F("LOG: "));
		//Serial.print(isEvent);
		//Serial.print(F(" - "));
		//Serial.println(stringToSave);
	}
	// if the file isn't open, pop up an error:
	else {
		//Serial.print("error opening file - ");
		//Serial.print(F("LOG ERROR: "));
		//Serial.print(isEvent);
		//Serial.print(F(" - "));
		//Serial.println(stringToSave);
	}

#endif
}