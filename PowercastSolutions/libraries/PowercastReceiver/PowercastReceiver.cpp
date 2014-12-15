#include "PowercastReceiver.h"

PowercastReceiver::PowercastReceiver()
{
	xbee = new XBee();
	//xbee->setSerial(Serial);
	rx16 = Rx16Response();
	txStatusResponse = TxStatusResponse();
	powercastControlLayer = NULL;
	powercastMeasurementLayer = NULL;

	enableSending = true;
}

void PowercastReceiver::setSerial(Stream &serial)
{
	xbee->setSerial(serial);
}

void PowercastReceiver::setControlLayer(PowercastLayer* powercastLayer)
{
	powercastControlLayer = powercastLayer;
	powercastControlLayer->setParentLayer(this);
}

void PowercastReceiver::setMeasurementLayer(PowercastLayer* powercastLayer)
{
	powercastMeasurementLayer = powercastLayer;
	powercastMeasurementLayer->setParentLayer(this);
}

void PowercastReceiver::setMeasurementObject(MeasurementObject *measObject)
{
	measurementObject = measObject;
}


boolean PowercastReceiver::tryReceivePacket()
{
	
	xbee->readPacket();

	if (xbee->getResponse().isAvailable()) {
		// got something

		if (xbee->getResponse().getApiId() == RX_16_RESPONSE) {
			// got a rx packet
			//Serial.println("Got an RX packet");
			xbee->getResponse().getRx16Response(rx16);
			return true;
		}
		else if (xbee->getResponse().getApiId() == TX_STATUS_RESPONSE) {
			xbee->getResponse().getTxStatusResponse(txStatusResponse);

			//TODO: bug here!!!
			if (measurementObject != NULL) {
				measurementObject->savePacketAckToFile(txStatusResponse.isSuccess());
			}

			/*
			if (txStatusResponse.isSuccess()) {
				//Serial.println("HOOORAY!!");
				measurementObject->savePacketAckToFile(txStatusResponse.isSuccess());
			}
			else {
				Serial.println("BUUUUUU!!");
			}*/
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

ProtocolPacket* PowercastReceiver::obtainPacket()
{
	//Serial.println("obtainPacket");
	
	receivePacket.senderAddress = rx16.getRemoteAddress16();

	byte* receivedPacketData = rx16.getData();
	byte receivedPacketDataLength = rx16.getDataLength();

	//Serial.print("obtainPacket - receivedPacketData: ");
	//Serial.println(*receivedPacketData, HEX);

	//Serial.print("obtainPacket - receivedPacketDataLength: ");
	//Serial.println(receivedPacketDataLength);

	receivePacket.protocolType = receivedPacketData[0];
	receivePacket.packetType = receivedPacketData[1];
	
	//receivePacket.packetData = new byte[receivedPacketDataLength - 2];

	int index = 0;
	for (int i = 2; i < receivedPacketDataLength; i++)
	{
		receivePacket.packetData[index++] = receivedPacketData[i];
		
		//Serial.print("obtainPacket - index (loop): ");
		//Serial.println(index);
		//Serial.print("obtainPacket - i (loop): ");
		//Serial.println(i);
		//Serial.print("obtainPacket - receivedPacketData (loop): ");
		//Serial.println(receivedPacketData[i], HEX);
	}

	receivePacket.packetDataLength = index;

	receivePacket.receivedPacketRssi = rx16.getRssi();
	//Serial.print("obtainPacket - index: ");
	//Serial.println(index);

	//Serial.print("obtainPacket - packetDataLength: ");
	//Serial.println(receivePacket.packetDataLength);

	//Serial.print("obtainPacket - senderAddress: ");
	//Serial.println(receivePacket.senderAddress, HEX);

	return &receivePacket;
}


void PowercastReceiver::executeProtocol()
{
	//Serial.println("executeProtocol");
	
	boolean isPacketReceived = tryReceivePacket();

	
	if (isPacketReceived) {
		//Serial.println("executeProtocol - PacketReceived");
		ProtocolPacket* packet = obtainPacket();

		if (powercastMeasurementLayer != NULL && packet->protocolType == PROTOCOL_TIMESYNCH) {
			powercastMeasurementLayer->processPacket(packet);
		}
		else if (powercastControlLayer != NULL && packet->protocolType == PROTOCOL_CONTROL) {
			powercastControlLayer->processPacket(packet);
		}

	}

	if (powercastMeasurementLayer != NULL) {
		powercastMeasurementLayer->doTimerActions();
	}

	if (powercastControlLayer != NULL) {
		powercastControlLayer->doTimerActions();
	}
	
}


void PowercastReceiver::sendPacketToAddress(ProtocolPacket* packetToSend, uint16_t address)
{
	//Serial.println("sendPacketToAddress");
	//Serial.println(address, HEX);
	// 16-bit addressing: Enter address of remote XBee, typically the coordinator
	Tx16Request tx = Tx16Request(address, packetToSend->getBytes(), packetToSend->getPacketDataSize());
	//Serial.println("before send");
	if (enableSending) {
		xbee->send(tx);
	}
	//Serial.println("sent");
}

void PowercastReceiver::sendPacketBroadcast(ProtocolPacket* packetToSend)
{
	sendPacketToAddress(packetToSend, ADDRESS_BROADCAST);
}
