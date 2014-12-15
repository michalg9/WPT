#include <XBee.h>
#include <PowercastComm.h>

PowercastComm powercastComm = PowercastComm();

// include the library code:
//#include <LiquidCrystal.h>

#include <SD.h>
const int chipSelect = 4;

char filenameArray[13];
#define LOG_FILENAME_PREFIX "MEASURE"

#define DELAY_PING 1000UL
#define DELAY_LCD 1000UL
#define DELAY_TIME_SYNCH 2000UL
#define DELAY_MEASUTEMENT 1000UL

// initialize the library with the numbers of the interface pins
//LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int sourcePin = 9;
int sourcePinValue = 0;

int averagePin = 6;
int averagePinValue = 0;

int referencePin = 8;
int referencePinValue = 0;

//Pin that sets dIn pin of Powercast
int dInPin = 7;

// Input pin for Dout output of Powercast
#define DOUT_INPUT_PIN A0
// Input pin for Vout output of the powercast
#define LOAD_INPUT_PIN A3

// Currently used output pin
int inputPin = DOUT_INPUT_PIN;

//Measured values
int internalReferenceValMv = 1100;
int defaultReferenceValMv = 5000;

voltageMeasurementStruct voltageMeasurementData;

void setup()
{
	// initialize serial communication with computer:
	Serial.begin(9600);
	powercastComm.setSerial(Serial);
	powercastComm.setReferencePointers(&internalReferenceValMv, &defaultReferenceValMv);

	initializeReadings();

	pinMode(sourcePin, INPUT);
	pinMode(averagePin, INPUT);
	pinMode(referencePin, INPUT);
	pinMode(dInPin, OUTPUT);

	digitalWrite(sourcePin, HIGH);
	digitalWrite(referencePin, HIGH);

	setupMeasurementSource(digitalRead(sourcePin));
	setupReferenceSource(digitalRead(referencePin));

	//setupLcd();


	pinMode(10, OUTPUT);
	if (!SD.begin(chipSelect)) {
		Serial.println("Card failed, or not present");
		// don't do anything more:
		return;
	}

	createNewFile();

}
/*
void setupLcd() {
// set up the LCD's number of columns and rows:
lcd.begin(16, 2);
// Print a message to the LCD.
lcd.print("V[mV]:");
lcd.setCursor(0, 1);
lcd.print("smpl:");
}*/

void setupMeasurementSource(int sourcePinValue) {
	if (sourcePinValue == 0) {
		digitalWrite(dInPin, HIGH);
		inputPin = DOUT_INPUT_PIN;
	}
	else {
		digitalWrite(dInPin, LOW);
		inputPin = LOAD_INPUT_PIN;
	}

}

void setupReferenceSource(int referencePinValue) {
	if (referencePinValue == 0) {
		analogReference(INTERNAL);
		voltageMeasurementData.referenceValMv = internalReferenceValMv;
	}
	else {
		analogReference(DEFAULT);
		voltageMeasurementData.referenceValMv = defaultReferenceValMv;
	}

}

int i = 0;
unsigned long timerPing = 0;
unsigned long timerLcd = 0;
unsigned long timerTimeSynch = 0;
unsigned long timerMeasutement = 0;
void loop() {

	readInputSettingsPins();

	if (millis() - timerPing >= DELAY_PING) {
		Serial.println("ping");
		powercastComm.sendPing();
		timerPing = millis();
	}

	if (millis() - timerTimeSynch >= DELAY_TIME_SYNCH) {
		Serial.println("timeSynch");
		powercastComm.broadcastTimeSynch();
		timerTimeSynch = millis();
	}

	if (millis() - timerMeasutement >= DELAY_MEASUTEMENT) {
		Serial.println("measurement");
		saveValue(voltageMeasurementData.calculatedVoltageMv);
		timerMeasutement = millis();
	}

	voltageMeasurementData.usedSampleValue = readValue();

	// Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
	//float voltage = usedValue * (referenceValMv / 1023.0);
	voltageMeasurementData.calculatedVoltageMv = voltageMeasurementData.usedSampleValue * (voltageMeasurementData.referenceValMv / 1023.0);

	boolean isPacketReceived = powercastComm.tryReceivePacket();
	if (isPacketReceived) {
		powercastComm.processPacket(&voltageMeasurementData);
	}

	/*
	if (millis() - timerLcd >= DELAY_LCD) {
	displayOnLcd(voltageMeasurementData.calculatedVoltageMv, voltageMeasurementData.usedSampleValue);
	timerLcd = millis();
	}*/

	delay(1);        // delay in between reads for stability         
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
	// make a string for assembling the data to log:
	String dataString = "";
	dataString += millis();
	dataString += ",";
	dataString += String(value);
	//dataString += ",";
	//dataString += String(voltageMeasurementData.referenceValMv);

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

/*
void displayOnLcd(int voltage, int usedValue) {
// (note: line 1 is the second row, since counting begins with 0):
lcd.setCursor(7, 0);
// print the number of seconds since reset:
//lcd.print(voltage);
lcd.print("     ");
lcd.setCursor(7, 0);
lcd.print(voltage);

lcd.setCursor(7, 1);
lcd.print("     ");
lcd.setCursor(7, 1);
lcd.print(usedValue);

}*/
void readInputSettingsPins() {
	int tempSourcePinValue = digitalRead(sourcePin);
	if (sourcePinValue != tempSourcePinValue) {
		Serial.println("Source pin changed");
		sourcePinValue = tempSourcePinValue;
		setupMeasurementSource(sourcePinValue);
	}

	averagePinValue = digitalRead(averagePin);

	int tempReferencePinValue = digitalRead(referencePin);
	if (referencePinValue != tempReferencePinValue) {
		Serial.println("Reference changed");
		referencePinValue = tempReferencePinValue;
		setupReferenceSource(referencePinValue);
	}
}

int readValue() {
	// read from the sensor:  
	int sensorValue = analogRead(inputPin);
	int usedValue = sensorValue;

	/*
	int usedValue = -1;
	if (averagePinValue == 0) {
	usedValue = smoothValue(sensorValue);
	}
	else {
	usedValue = sensorValue;
	}*/

	return usedValue;
}

// Define the number of samples to keep track of.  The higher the number,
// the more the readings will be smoothed, but the slower the output will
// respond to the input.  Using a constant rather than a normal variable lets
// use this value to determine the size of the readings array.
const int numReadings = 32;

int readings[numReadings];      // the readings from the analog input
int index = 0;                  // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average

int smoothValue(int sensorValue) {
	// subtract the last reading:
	total = total - readings[index];
	// read from the sensor:
	readings[index] = sensorValue;
	// add the reading to the total:
	total = total + readings[index];
	// advance to the next position in the array:  
	index = index + 1;

	// if we're at the end of the array...
	if (index >= numReadings)
		// ...wrap around to the beginning: 
		index = 0;

	// calculate the average:
	average = total / numReadings;

	return average;
}

void initializeReadings() {
	// initialize all the readings to 0: 
	for (int thisReading = 0; thisReading < numReadings; thisReading++)
		readings[thisReading] = 0;
}






