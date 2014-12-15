#include <PowercastReceiver.h>
#include <XBee.h>
#include <PowercastComm.h>

#include <SD.h>
const int chipSelect = 4;

char filenameArray[13];
#define LOG_FILENAME_PREFIX "MEASURE"

PowercastComm powercastComm = PowercastComm();
int pinTxOff = 10;
int pinTxOn = 9;

#define DELAY_MEASUTEMENT 100UL
unsigned long timerMeasutement = 0;

void setup()
{
	Serial.begin(9600);
	powercastComm.setSerial(Serial);
	pinMode(pinTxOff, OUTPUT);
	pinMode(pinTxOn, OUTPUT);
	digitalWrite(pinTxOff, HIGH);
	digitalWrite(pinTxOn, LOW);
	powercastComm.setOutputPins(pinTxOff, pinTxOn);

	pinMode(10, OUTPUT);
	if (!SD.begin(chipSelect)) {
		Serial.println("Card failed, or not present");
		// don't do anything more:
		return;
	}

	createNewFile();
}

void loop()
{
	powercastComm.checkForPing();
	boolean isPacketReceived = powercastComm.tryReceivePacket();

	if (isPacketReceived) {
		voltageMeasurementStruct test = {
			0, 0, 0 };
		powercastComm.processPacket(&test);
	}

	if (millis() - timerMeasutement >= DELAY_MEASUTEMENT) {
		Serial.println("measurement");
		saveValue(powercastComm.getTxStatus());
		timerMeasutement = millis();
	}
}


void createNewFile() {

	for (int i = 1; i < 10; i++) {
		String filename = "";
		filename += LOG_FILENAME_PREFIX;
		filename += i;
		filename += ".txt";
		filename.toCharArray(filenameArray, 13);
		if (!SD.exists(filenameArray)) {
			break;
		}
	}

	Serial.print("New filename: ");
	Serial.println(filenameArray);
}

void saveValue(int value) {
	if (powercastComm.recentReceivedTimestamp != 0) {
		// make a string for assembling the data to log:
		String dataString = "";
		dataString += String(millis() - powercastComm.localTimestampOnSynch + powercastComm.recentReceivedTimestamp);
		dataString += ",";
		dataString += String(value);

		// open the file. note that only one file can be open at a time,
		// so you have to close this one before opening another.
		File dataFile = SD.open(filenameArray, FILE_WRITE);

		// if the file is available, write to it:
		if (dataFile) {
			dataFile.println(dataString);
			dataFile.close();
			// print to the serial port too:
			Serial.println(dataString);
		}
		// if the file isn't open, pop up an error:
		else {
			Serial.println("error opening file");
		}
	}
}

