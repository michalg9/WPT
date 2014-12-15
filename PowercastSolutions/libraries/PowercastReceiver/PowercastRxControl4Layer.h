#ifndef PowercastRxControl4Layer_h
#define PowercastRxControl4Layer_h

#include "PowercastLayer.h"

#define IS_TX false

#define TYPE_PING 0
#define TYPE_VOLTAGE_REQUEST 1
#define TYPE_VOLTAGE_RESPONSE 2

#define STATE_IDLE 0
#define STATE_WAITING_FOR_POWER 1
#define STATE_CHARGING 2

#define EVENT_TIMEOUT_PING 0
#define EVENT_POWER_PROBE_REQUEST_RECEIVED_NOT_FROM_QUEUE_ADDRESS 1
#define EVENT_VOLTAGE_ABOVE_THRESHOLD 2
#define EVENT_TIMEOUT_POWER_PROBING 3
#define EVENT_TIMEOUT_WAITING_FOR_POWER 4
#define EVENT_TIMEOUT_REMOVE_OLDEST 5
#define EVENT_VOLTAGE_BELOW_THRESHOLD 6


#define ACTION_SEND_PING 0
#define ACTION_SEND_POWER_PROBE 1
#define ACTION_SAVE_RECEIVED_POWER_PROBE_ADDRESS 2
#define ACTION_REMOVE_OLDEST_POWER_PROBE_ADDRESS 3

#include "PowercastControlParameters.h"

typedef struct
{
	int referenceValMv;
	int calculatedVoltageMv;
	int usedSampleValue;
} voltageMeasurementStruct;

#define SIZE_OF_TX_QUEUE 4


class PowercastRxControl4Layer: public PowercastLayer
{
	class TxAdressesQueue
	{
	public:
		TxAdressesQueue()
		{
			resetQueue();
		}

		void resetQueue()
		{
			for (int i = 0; i < SIZE_OF_TX_QUEUE; i++)
			{
				txAdressesQueue[i] = 0;
				timestampsForRemovalQueue[i] = 0;
			}

			delayRemoveLastProbeSender = 0;
			
			currentIndex = 0;
			oldestIndex = 0;
			previousIndex = 0;
		}

		void saveAddressToQueue(uint16_t address)
		{
			Serial.print(millis());
			Serial.print(" Saving address ");
			Serial.print(address, HEX);
			Serial.print(" to queue on position ");
			Serial.println(currentIndex);

			printQueue();

			txAdressesQueue[currentIndex] = address;
			timestampsForRemovalQueue[currentIndex] = millis() + DELAY_REMOVE_LAST_PROBE_SENDER;
			previousIndex = currentIndex;
			currentIndex = (currentIndex + 1) % SIZE_OF_TX_QUEUE;
		}

		bool checkOldestTxForTimeout()
		{
			if (timestampsForRemovalQueue[oldestIndex] != 0 && timestampsForRemovalQueue[oldestIndex] <= millis()) {
				return true;
			}
			
			return false;
		}

		void removeOldestFromQueue()
		{
			Serial.print(millis());
			Serial.print(" Removing address ");
			Serial.print(txAdressesQueue[oldestIndex], HEX);
			Serial.print(" to queue on position ");
			Serial.println(oldestIndex);

			printQueue();

			txAdressesQueue[oldestIndex] = 0;
			timestampsForRemovalQueue[oldestIndex] = 0;
			oldestIndex = (oldestIndex + 1) % SIZE_OF_TX_QUEUE;
		}

		bool checkForAddress(uint16_t address)
		{
			for (int i = 0; i < SIZE_OF_TX_QUEUE; i++)
			{
				if (txAdressesQueue[i] == address)
				{
					return true;
				}
			}

			return false;
		}

		bool checkForLastAddress(uint16_t address)
		{
			/*Serial.print("Checking address  ");
			Serial.print(address, HEX);
			Serial.print(" with address ");
			Serial.print(txAdressesQueue[indexToCheck], HEX);
			Serial.print(" on position ");
			Serial.println(indexToCheck);*/

			if (txAdressesQueue[previousIndex] == address)
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		uint16_t getLastAddress()
		{
			return txAdressesQueue[previousIndex];
		}

		bool printQueue()
		{
			Serial.println("Delay pointer");
			Serial.println(delayRemoveLastProbeSender);
			Serial.print("Queue print at ts ");
			Serial.println(millis());
			for (int i = 0; i < SIZE_OF_TX_QUEUE; i++)
			{
				Serial.print("Pos: ");
				Serial.print(i);
				Serial.print(" Addr: ");
				Serial.println(txAdressesQueue[i], HEX);
				Serial.print(" Ts to remove: ");
				Serial.println(timestampsForRemovalQueue[i]);
			}

			return false;
		}

	private:
		uint16_t txAdressesQueue[SIZE_OF_TX_QUEUE];
		unsigned long timestampsForRemovalQueue[SIZE_OF_TX_QUEUE];
		unsigned long delayRemoveLastProbeSender;
		byte currentIndex;
		byte oldestIndex;
		byte previousIndex;
	};

public:
	PowercastRxControl4Layer();
	void resetMeasurement();

	void processPacket(ProtocolPacket *xBeePacket);
	void doTimerActions();
	void processState(Event currentEvent, ProtocolPacket *xBeePacket);

	bool isTheAddressAllowed(uint16_t address);

	//EVENTS
	void eventTimeoutPing();
	void eventPowerProbeRequestReceivedNotFromQueueAdress(ProtocolPacket *xBeePacket);
	void eventVoltageAboveThreshold();
	void eventTimeoutPowerProbing();
	void eventTimeoutWaitingForPower();
	void eventTimeoutRemoveOldestProbeSender();
	void eventVoltageBelowThreshold();

	// ACTIONS
	void actionSendPing();
	void actionSendPowerProbe(ProtocolPacket *xBeePacket);
	void actionSaveReceivedPowerProbeAddress(ProtocolPacket *xBeePacket);
	void actionRemoveOldestPowerProbeAddress();

	voltageMeasurementStruct* voltageMeasurementStructData;

	void doMeasurement();

private:
	TxAdressesQueue txAddressQueue;

	unsigned long timerPing;
	unsigned long timerProbing;
	unsigned long timerWaitingForPower;
};

#endif