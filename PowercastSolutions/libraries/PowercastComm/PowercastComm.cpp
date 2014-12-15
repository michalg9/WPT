#include "Arduino.h"
#include <XBee.h>
#include "PowercastComm.h"

PowercastComm::PowercastComm()
{
	xbee = new XBee();
	rx16 = Rx16Response();
	txStatusResponse = TxStatusResponse ();
	lastPingReceived = 0;
	txStatus = false;
	
	recentReceivedTimestamp = 0;
	localTimestampOnSynch = 0;
	
	rssiThreshold = RSSI_OFF_THRESHOLD;

	powercastState = Off;
}

void PowercastComm::setSerial(Stream &serial)
{
	xbee->setSerial(serial);
}

boolean PowercastComm::tryReceivePacket()
{
	xbee->readPacket();

  if (xbee->getResponse().isAvailable()) {
    // got something
	
    if (xbee->getResponse().getApiId() == RX_16_RESPONSE) {
      // got a rx packet
      xbee->getResponse().getRx16Response(rx16);
      return true;
    }
	else if (xbee->getResponse().getApiId() == TX_STATUS_RESPONSE) {
		xbee->getResponse().getTxStatusResponse(txStatusResponse);
		
		if (txStatusResponse.isSuccess()) {
			Serial.println("HOOORAY!!");
		}
		else {
			Serial.println("BUUUUUU!!");
		}
	}
    else {
      return false;
    }
  } 
  else if (xbee->getResponse().isError()) {
    //nss.print("Error reading packet.  Error code: ");  
    //nss.println(xbee->getResponse().getErrorCode());
    // or flash error led
    return false;
  } 
  
  return false;
}
/*
void PowercastComm::processPacket()
{
  byte* packetData = rx16.getData();
  byte dataLength = rx16.getDataLength();
  
  int packetType = rx16.getData(0);
  Serial.println(packetType);
  if (packetType == TYPE_VOLTAGE_REQUEST) {
    int sequenceNumber = rx16.getData(1);
    Serial.println("Send voltage report");
	voltageMeasurementStruct* test;
	
    sendVoltageReport(sequenceNumber, test);
  }
  else if (packetType == TYPE_RSSI_REQUEST) {
    int sequenceNumber = rx16.getData(1);
    Serial.println("Send RSSI ping");
    sendRssiPing(sequenceNumber);
  }
  else if (packetType == TYPE_PING) {
    Serial.println("Received ping");
  }
  else if (packetType == TYPE_RSSI_PING) {
	Serial.println("RECEIVED RSSI PING LALALALALA");
    int sequenceNumber = rx16.getData(1);
    int rssi = rx16.getRssi();
	Serial.print("Sequence number: " );
    Serial.print(sequenceNumber);
	Serial.print(" RSSI: " );
    Serial.println(rssi);
    sendRssiReport(sequenceNumber, rssi);
  }
  else if (packetType == TYPE_TEST) {
  	Serial.println("TEST PACKET");
    int sequenceNumber = rx16.getData(1);
	sendTestResponse(sequenceNumber);
  }
}*/

void PowercastComm::processPacket(voltageMeasurementStruct* voltageMeasurementStructData)
{
  byte* packetData = rx16.getData();
  byte dataLength = rx16.getDataLength();
  
  byte packetType = packetData[0];
  Serial.println(packetType);
  if (packetType == TYPE_VOLTAGE_REQUEST) {
    byte sequenceNumber = packetData[1];
    Serial.println(voltageMeasurementStructData->referenceValMv);
    sendVoltageReport(sequenceNumber, voltageMeasurementStructData);
  }
  else if (packetType == TYPE_RSSI_REQUEST) {
    byte sequenceNumber = packetData[1];
    Serial.print("Send RSSI ping sequenceNumber ");
	Serial.println(sequenceNumber);
    sendRssiPing(sequenceNumber);
  }
  else if (packetType == TYPE_PING) {
    Serial.println("Received ping");

	byte rssi = rx16.getRssi();
	Serial.print(" RSSI: " );
    Serial.println(rssi);
	
	if (rssi < rssiThreshold) {
		lastPingReceived = millis();
		evaluateState(powercastState, PingReceived);
	}
  }
  else if (packetType == TYPE_RSSI_PING) {
	Serial.println("RECEIVED RSSI PING LALALALALA");
    byte sequenceNumber = packetData[1];
    byte rssi = rx16.getRssi();
	Serial.print("Sequence number: " );
    Serial.print(sequenceNumber);
	Serial.print(" RSSI: " );
    Serial.println(rssi);
    sendRssiReport(sequenceNumber, rssi);
  }
  else if (packetType == TYPE_TEST) {
    byte sequenceNumber = packetData[1];
	Serial.print("TEST PACKET sequence ");
	Serial.println(sequenceNumber);
	for (int i = 0; i < dataLength; i++) { 
         Serial.print(packetData[i], HEX);
		 Serial.print(" - "); 
       } 
	Serial.println("end");
	sendTestResponse(sequenceNumber);
  }
  else if (packetType == TYPE_CHANGE_REFERENCE) {
    byte sequenceNumber = packetData[1];
	int internalReference = makeInteger(packetData[2], packetData[3]);
    int defaultReference = makeInteger(packetData[4], packetData[5]);
	
	changeReference(internalReference, defaultReference);
	sendChangeReferenceResponse(sequenceNumber);
  }
  else if (packetType == TYPE_TIME_SYNCH) {
	
	byte byteArray[4];
	
	byteArray[0] = packetData[1];
	byteArray[1] = packetData[2];
	byteArray[2] = packetData[3];
	byteArray[3] = packetData[4];
	
	unsigned long longInt = convertByteArrayToLong(byteArray);
  
	recentReceivedTimestamp = longInt;
	localTimestampOnSynch = millis();
	
	Serial.println("Received timestamp synch:");
	Serial.println(recentReceivedTimestamp);
	Serial.println("Current timestamp:");
	Serial.println(localTimestampOnSynch);
  
  }
  else if (packetType == TYPE_SET_RSSI_THRESHOLD_REQUEST) {
  Serial.println("sadsadsadasdasdasd");
	byte sequenceNumber = packetData[1];
	rssiThreshold = packetData[2];
	
	Serial.println("Threshold changed ");
	Serial.println(rssiThreshold);
  
  }
}

void PowercastComm::evaluateState(PowercastState currentState, PowercastEvent currentEvent) {
	
	PowercastState nextState;
	Serial.print("---CURRENT STATE: ");
	Serial.print(currentState);
	Serial.print("---CURRENT EVENT: ");
	Serial.print(currentEvent);
	switch (currentState)
	{
	case Off :
		if (currentEvent == PingReceived)
		{
			nextState = On;
		}
		else
		{
			nextState = Off;
		}
		break;
	case On:
		if (currentEvent == PingReceivedTimeout)
		{
			nextState = Off;
		}
		else
		{
			nextState = On;
		}
		break;
	default:
		break;
	}

	powercastState = nextState;
	Serial.print("---NEXT STATE: ");
	Serial.print(nextState);
	if (nextState != currentState) {
		applyState(powercastState);
	}
	//makeTxOn(false);
}

void PowercastComm::applyState(PowercastState currentState) {
	Serial.println("-----------------APPLY STATE");
	switch (currentState)
	{
	case Off:
		Serial.println("################  TURN OFF");
		makeTxOn(false);
		break;
	case On:
		Serial.println("################  TURN ON");
		makeTxOn(true);
		//if (!txStatus) {
		//	makeTxOn(true);
		//}
		break;
	default:
		break;
	}

}

void PowercastComm::sendVoltageReport(byte sequenceNumber, voltageMeasurementStruct* voltageMeasurementStructData)
{
  // Frame format:
  // 1st byte - TYPE
  // 2nd byte - sequence number
  // 3nd and 4rd byte - voltage [mV]
  // 5th and 6th byte - sample number (0-1023)
  // 7th and 8th - voltage reference used [mV]
  uint8_t payload[] = {0, 0, 0, 0, 0, 0, 0, 0};
  payload[0] = TYPE_VOLTAGE_REPORT;
  payload[1] = sequenceNumber;
  payload[2] = highByte(voltageMeasurementStructData->calculatedVoltageMv);
  payload[3] = lowByte(voltageMeasurementStructData->calculatedVoltageMv);
  payload[4] = highByte(voltageMeasurementStructData->usedSampleValue);
  payload[5] = lowByte(voltageMeasurementStructData->usedSampleValue);
  payload[6] = highByte(voltageMeasurementStructData->referenceValMv);
  payload[7] = lowByte(voltageMeasurementStructData->referenceValMv);

  // 16-bit addressing: Enter address of remote XBee, typically the coordinator
  Tx16Request tx = Tx16Request(ADDRESS_PC, payload, sizeof(payload));
  xbee->send(tx);
}
	

void PowercastComm::sendRssiStartPing(byte sequenceNumber, int numberOfValues)
{
  // Frame format:
  // 1st byte - TYPE
  // 2nd byte - sequence number

  uint8_t payload[] = {0, 0};
  payload[0] = TYPE_START_RSSI_PING;
  payload[1] = sequenceNumber;
  
  // 16-bit addressing: Enter address of remote XBee, typically the coordinator
  Tx16Request tx = Tx16Request(ADDRESS_BROADCAST, payload, sizeof(payload));
  xbee->send(tx);
}

void PowercastComm::sendRssiStopPing(byte sequenceNumber)
{
  // Frame format:
  // 1st byte - TYPE
  // 2nd byte - sequence number

  uint8_t payload[] = {0, 0};
  payload[0] = TYPE_STOP_RSSI_PING;
  payload[1] = sequenceNumber;
  
  // 16-bit addressing: Enter address of remote XBee, typically the coordinator
  Tx16Request tx = Tx16Request(ADDRESS_BROADCAST, payload, sizeof(payload));
  xbee->send(tx);
}
	
void PowercastComm::sendRssiPing(byte sequenceNumber)
{
  // Frame format:
  // 1st byte - TYPE
  // 2nd byte - sequence number

  uint8_t payload[] = {0, 0};
  payload[0] = TYPE_RSSI_PING;
  payload[1] = sequenceNumber;
  
  // 16-bit addressing: Enter address of remote XBee, typically the coordinator
  Tx16Request tx = Tx16Request(ADDRESS_BROADCAST, payload, sizeof(payload));
  xbee->send(tx);
}

void  PowercastComm::sendPing()
{
	Serial.println("ping");
  // Frame format:
  // 1st byte - TYPE

  uint8_t payload[] = {0};
  payload[0] = TYPE_PING;  
  // 16-bit addressing: Enter address of remote XBee, typically the coordinator
  Tx16Request tx = Tx16Request(ADDRESS_BROADCAST, payload, sizeof(payload));
  xbee->send(tx);
}	


void PowercastComm::sendRssiReport(byte sequenceNumber, byte rssiValue)
{

  // Frame format:
  // 1st byte - TYPE
  // 2nd byte - request sequenceNumber
  // 3rd yte - rssiValue [dBm] (value is below 0)
  uint8_t payload[] = {
    0, 0, 0  };
  payload[0] = TYPE_RSSI_REPORT;
  payload[1] = sequenceNumber;
  payload[2] = rssiValue;

  // 16-bit addressing: Enter address of remote XBee, typically the coordinator
  Tx16Request tx = Tx16Request(ADDRESS_PC, payload, sizeof(payload));
  xbee->send(tx);
}

void PowercastComm::sendRssiReport(byte sequenceNumber, byte rssiValues[], int numOfValues) {
  // Frame format:
  // 1st byte - TYPE
  // 2nd byte - request sequenceNumber
  // 3rd yte - rssiValue [dBm] (value is below 0)
  uint8_t payload[2+numOfValues];
  payload[0] = TYPE_RSSI_REPORT;
  payload[1] = sequenceNumber;
  for (int i = 0; i < numOfValues; i++) {
	payload[2+i] = rssiValues[i];
  }

  // 16-bit addressing: Enter address of remote XBee, typically the coordinator
  Tx16Request tx = Tx16Request(ADDRESS_PC, payload, sizeof(payload));
  xbee->send(tx);
}


void PowercastComm::sendTestResponse(byte sequenceNumber)
{

  // Frame format:
  // 1st byte - TYPE
  // 2nd byte - request sequenceNumber
  uint8_t payload[] = {
    0, 0};
  payload[0] = TYPE_TEST_RESPONSE;
  payload[1] = sequenceNumber;

  // 16-bit addressing: Enter address of remote XBee, typically the coordinator
  Tx16Request tx = Tx16Request(ADDRESS_PC, payload, sizeof(payload));
  xbee->send(tx);

}

void PowercastComm::sendChangeReferenceResponse(byte sequenceNumber)
{
  // Frame format:
  // 1st byte - TYPE
  // 2nd byte - request sequenceNumber
  // 3rd and 4th byte - internal reference value
  // 5th and 6th byte - default reference value
  uint8_t payload[] = {
    0, 0, 0, 0, 0, 0};
  payload[0] = TYPE_CHANGE_REFERENCE_RESPONSE;
  payload[1] = sequenceNumber;
  payload[2] = highByte(*internalReferenceValMv);
  payload[3] = lowByte(*internalReferenceValMv);
  payload[4] = highByte(*defaultReferenceValMv);
  payload[5] = lowByte(*defaultReferenceValMv);

  // 16-bit addressing: Enter address of remote XBee, typically the coordinator
  Tx16Request tx = Tx16Request(ADDRESS_PC, payload, sizeof(payload));
  xbee->send(tx);
}

void PowercastComm::broadcastTimeSynch() {

	Serial.println("timeSynch");

  // Frame format:
  // 1st byte - TYPE
  // 2nd, 3rd, 4th and 5th byte - timestamp
  uint8_t payload[] = {
    0, 0, 0, 0, 0};
  payload[0] = TYPE_TIME_SYNCH;
  
  byte byteArray[4];
  unsigned long currentTimestamp = millis();
  convertLongToByteArray(currentTimestamp, byteArray);
  
  payload[1] = byteArray[0];
  payload[2] = byteArray[1];
  payload[3] = byteArray[2];
  payload[4] = byteArray[3];

  Serial.println("Broadcasting timestamp:");
  Serial.println(currentTimestamp);
  
  // 16-bit addressing: Enter address of remote XBee, typically the coordinator
  Tx16Request tx = Tx16Request(ADDRESS_BROADCAST, payload, sizeof(payload));
  xbee->send(tx);
}

void PowercastComm::changeReference(int internalReferenceValue, int defaultReferenceValue)
{
	Serial.println("Changing reference");
	Serial.print("Old internal: ");
	Serial.print(*internalReferenceValMv);
	Serial.print(" New internal: ");
	Serial.println(internalReferenceValue);
	Serial.print("Old default: ");
	Serial.print(*defaultReferenceValMv);
	Serial.print(" New default: ");
	Serial.println(defaultReferenceValue);
	
	*internalReferenceValMv = internalReferenceValue;
	*defaultReferenceValMv = defaultReferenceValue;
	
	Serial.println("Success");
}

void PowercastComm::setReferencePointers(int* internalReferenceValMvArduino, int* defaultReferenceValMvArduino)
{
	internalReferenceValMv = internalReferenceValMvArduino;
	defaultReferenceValMv = defaultReferenceValMvArduino;
}

void PowercastComm::setOutputPins(int pinTxOff, int pinTxOn)
{
	this->pinTxOff = pinTxOff;
	this->pinTxOn = pinTxOn;
}

void PowercastComm::makeTxOn(bool status)
{
	if (status) {
		digitalWrite(pinTxOff, LOW);
		digitalWrite(pinTxOn, HIGH);
		txStatus = true;
	}
	else {
		digitalWrite(pinTxOff, HIGH);
		digitalWrite(pinTxOn, LOW);
		txStatus = false;
	}
}

void PowercastComm::checkForPing()
{
	if (millis() - lastPingReceived > PING_THRESHOLD) {
		evaluateState(powercastState, PingReceivedTimeout);
	}
}

void PowercastComm::convertLongToByteArray(long longInt, byte byteArray[]) {
  
  byteArray[0] = (byte) ((longInt >> 24) & 0xFF) ;
  byteArray[1] = (byte) ((longInt >> 16) & 0xFF) ;
  byteArray[2] = (byte) ((longInt >> 8) & 0XFF);
  byteArray[3] = (byte) ((longInt & 0XFF));

}

long PowercastComm::convertByteArrayToLong(byte byteArray[]) {
  unsigned long longInt;

  longInt  = ((unsigned long) byteArray[0]) << 24;
  longInt |= ((unsigned long) byteArray[1]) << 16;
  longInt |= ((unsigned long) byteArray[2]) << 8;
  longInt |= ((unsigned long) byteArray[3]);
  
  return longInt;
}

bool PowercastComm::getTxStatus() {
	return txStatus;
}