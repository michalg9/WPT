#define NO_MEASUREMENT 1
//#define BEACONING 1
//#define TEST_SUITE 1


#ifndef NO_MEASUREMENT
#include <SPI.h>
#include <SD.h>
#endif

//#include <MemoryFree.h>
#include <XBee.h>

#include <PowercastReceiver.h>

#ifdef BEACONING
#include <PowercastTxControlLayer.h>
#else
#include <PowercastTxControl3Layer.h>
#endif

#ifndef NO_MEASUREMENT
#include <PowercastTxMeasurementLayer.h>
#include <MeasurementObject.h>
#endif

PowercastReceiver powercastComm = PowercastReceiver();
#ifdef BEACONING
PowercastTxControlLayer powercastControlLayer = PowercastTxControlLayer();
#else
PowercastTxControl3Layer powercastControlLayer = PowercastTxControl3Layer();
#endif

// setting up Measurements
#ifndef NO_MEASUREMENT
PowercastTxMeasurementLayer powercastMeasurementLayer = PowercastTxMeasurementLayer();
MeasurementObject txMeasurementObject = MeasurementObject();
#endif

int pinTxOff = 10;
int pinTxOn = 9;

void setup()
{
	// initialize serial communication with computer:.

	Serial.begin(9600);
	//Serial.println(F("Initialized"));
	powercastComm.setSerial(Serial);

	pinMode(pinTxOff, OUTPUT);
	pinMode(pinTxOn, OUTPUT);
	digitalWrite(pinTxOff, HIGH);
	digitalWrite(pinTxOn, LOW);
	powercastControlLayer.setOutputPins(pinTxOff, pinTxOn);

	// setting up Measurement
#ifndef NO_MEASUREMENT
	txMeasurementObject.initializeObject();
	powercastControlLayer.setMeasurementObject(&txMeasurementObject);
	powercastMeasurementLayer.setMeasurementObject(&txMeasurementObject);
	powercastComm.setMeasurementObject(&txMeasurementObject);
	powercastComm.setMeasurementLayer(&powercastMeasurementLayer);
#endif

	// setting up Control
	powercastComm.setControlLayer(&powercastControlLayer);
	

	// initialization of random seed
	randomSeed(analogRead(0));
}

int loopNumber = 0;

void loop() {
	//Serial.print(F("Loop "));
	//Serial.print(++loopNumber);
	//Serial.print(F(" t: "));
	//Serial.print(millis());
	//Serial.print(F(" freeMemory()= "));
	//Serial.println(freeMemory());

	powercastComm.executeProtocol();
	//powercastControlLayer.printState();
	//Serial.print(" freeMemory2()= ");
	//Serial.println(freeMemory());
	//Serial.println();
	//Serial.println();
	//Serial.println();
	//delay(1);

}






