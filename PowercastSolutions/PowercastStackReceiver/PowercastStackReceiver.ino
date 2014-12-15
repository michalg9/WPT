
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
#include <PowercastRxControlLayer.h>
#else
#include <PowercastRxControl4Layer.h>
#endif

#ifndef NO_MEASUREMENT
#include <PowercastRxMeasurementLayer.h>
#include <MeasurementObject.h>
#endif


// setting up Control
PowercastReceiver powercastComm = PowercastReceiver();

#ifdef BEACONING
PowercastRxControlLayer powercastControlLayer = PowercastRxControlLayer();
#else
PowercastRxControl4Layer powercastControlLayer = PowercastRxControl4Layer();
#endif

// setting up Measurements
#ifndef NO_MEASUREMENT
PowercastRxMeasurementLayer powercastMeasurementLayer = PowercastRxMeasurementLayer();
MeasurementObject rxMeasurementObject = MeasurementObject();
#endif

// Input pin for Dout output of Powercast
#define DOUT_INPUT_PIN A0
// Input pin for Vout output of the powercast
#define LOAD_INPUT_PIN A3

// Currently used output pin
int inputPin = DOUT_INPUT_PIN;

#define REFERENCE_VAL_MV_INTERNAL 1100
#define REFERENCE_VAL_MV_DEFAULT 5000


voltageMeasurementStruct voltageMeasurementData;

// --- Button
// constants won't change. They're used here to 
// set pin numbers:
const int buttonPin = 2;    // the number of the pushbutton pin
const int ledPin = 10;      // the number of the LED pin

// Variables will change:
int ledState = LOW;         // the current state of the output pin
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin

// the following variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 50;    // the debounce time; increase if the output flickers
// --- Button
bool isMeasuring = false;
void setup()
{
	// initialize serial communication with computer:
	Serial.begin(9600);
	//Serial.println(F("Initialized"));
	powercastComm.setSerial(Serial);

	voltageMeasurementData.referenceValMv = REFERENCE_VAL_MV_DEFAULT;

	powercastControlLayer.voltageMeasurementStructData = &voltageMeasurementData;

	// setting up Measurement
#ifndef NO_MEASUREMENT
	rxMeasurementObject.initializeObject();
	powercastControlLayer.setMeasurementObject(&rxMeasurementObject);
	powercastMeasurementLayer.setMeasurementObject(&rxMeasurementObject);
	powercastComm.setMeasurementObject(&rxMeasurementObject);
	powercastComm.setMeasurementLayer(&powercastMeasurementLayer);
#endif

	// setting up Control
	powercastComm.setControlLayer(&powercastControlLayer);
	

	// button debouncing
	pinMode(buttonPin, INPUT);
	pinMode(ledPin, OUTPUT);
	// set initial LED state
	digitalWrite(ledPin, ledState);


	randomSeed(100);
}




#ifdef TEST_SUITE
bool sendingEnabled = false;
#else
bool sendingEnabled = true;
#endif

//int loopNumber = 0;
//unsigned long executionTime;
void loop() {
	//Serial.print(F("Loop "));
	//Serial.print(++loopNumber);
	//Serial.print(F(" t: "));
	//executionTime = millis();
	//Serial.print(executionTime);
	//Serial.print(F(" freeMemory()= "));
	//Serial.println(freeMemory());

	//debounceButton();

#ifdef TEST_SUITE
	testSuite();
#endif

	if (sendingEnabled) {
		voltageMeasurementData.usedSampleValue = analogRead(inputPin);
	}
	else {
		voltageMeasurementData.usedSampleValue = 0;
	}
	

	// Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
	//float voltage = usedValue * (referenceValMv / 1023.0);
	voltageMeasurementData.calculatedVoltageMv = voltageMeasurementData.usedSampleValue * (voltageMeasurementData.referenceValMv / 1023.0);
	//Serial.println(voltageMeasurementData.calculatedVoltageMv);
	powercastComm.enableSending = sendingEnabled;
	powercastComm.executeProtocol();
	//Serial.print("Enabe sending ");
	//Serial.println(powercastComm.enableSending);
	//if (isMeasuring) {
	//	powercastComm.executeProtocol();
	//}

	
	//Serial.println();
	//Serial.println();
	//Serial.println();
	//Serial.print(F(" Time spent "));
	//Serial.println(millis()-executionTime);
	//delay(200);

}

#ifdef TEST_SUITE
unsigned long timeButtonHigh = 0;
bool measuringButtonTime = false;
//40
//20
//12
//10
//#define TEST_TIME_ON 40000U
//#define TEST_TIME_OFF 15000U
//#define TEST_TIME_ON_RANDOM  4000U
//#define TEST_COUNT_MAX 10

#define TEST_TIME_ON 20000U
#define TEST_TIME_OFF 10000U
#define TEST_TIME_ON_RANDOM  0U
#define TEST_COUNT_MAX 10

//#define TEST_TIME_ON 60000U
//#define TEST_TIME_OFF 20000U
//#define TEST_TIME_ON_RANDOM  20000U
//#define TEST_COUNT_MAX 10

unsigned long lastTimeTestChanged = 0;
long randNumber = 0;
byte testCount = 0;

#define TEST_SUITE_2_5MIN 300000U

void testSuite() {

	// appearing and disappearing test suite
	if (!sendingEnabled && millis() - lastTimeTestChanged > TEST_TIME_OFF) {
		startExperiment();
		lastTimeTestChanged = millis();

		if (testCount == TEST_COUNT_MAX) {
			//playSound();
			blinkLed();
		}

		testCount++;
	}
	
	if (sendingEnabled && millis() - lastTimeTestChanged >= TEST_TIME_ON + randNumber) {
		//randNumber = random(0, TEST_TIME_ON_RANDOM);
		stopExperiment();
		lastTimeTestChanged = millis();
		playSound();
	}


	// on for 5 min test suite
	/*if (!sendingEnabled) {
		startExperiment();
		lastTimeTestChanged = millis();
	}

	if (sendingEnabled && millis() - lastTimeTestChanged >= TEST_SUITE_2_5MIN) {
		stopExperiment();
		blinkLed();
	}*/

}

void startExperiment() {
	powercastControlLayer.resetMeasurement();
	sendingEnabled = true;

#ifndef NO_MEASUREMENT
	rxMeasurementObject.saveButtonEventToFile(true);
#endif

	ledState = HIGH;

	// set the LED:
	digitalWrite(ledPin, ledState);

}

void stopExperiment() {
	sendingEnabled = false;

#ifndef NO_MEASUREMENT
	rxMeasurementObject.saveButtonEventToFile(false);
#endif
	ledState = LOW;

	// set the LED:
	digitalWrite(ledPin, ledState);
}

void playSound() {
	for (int i = 1; i <= 5; i++) {
		tone(8, 784, 250);
		delay(500);
	}
	noTone(8);
}

void blinkLed() {
	while (true) {
		digitalWrite(ledPin, HIGH);   // turn the LED on (HIGH is the voltage level)
		delay(1000);               // wait for a second
		digitalWrite(ledPin, LOW);    // turn the LED off by making the voltage LOW
		delay(1000);               // wait for a second
	}
}

#endif


//void debounceButton() {
//
//	// read the state of the switch into a local variable:
//	int reading = digitalRead(buttonPin);
//
//	if (measuringButtonTime && millis() - timeButtonHigh > 2000U) {
//		measuringButtonTime = false;
//		stopExperiment();
//	}
//	// check to see if you just pressed the button 
//	// (i.e. the input went from LOW to HIGH),  and you've waited 
//	// long enough since the last press to ignore any noise:  
//
//	// If the switch changed, due to noise or pressing:
//	if (reading != lastButtonState) {
//		// reset the debouncing timer
//		lastDebounceTime = millis();
//	}
//
//	if ((millis() - lastDebounceTime) > debounceDelay) {
//		// whatever the reading is at, it's been there for longer
//		// than the debounce delay, so take it as the actual current state:
//
//		// if the button state has changed:
//		if (reading != buttonState) {
//			buttonState = reading;
//
//			// only toggle the LED if the new button state is HIGH
//			if (buttonState == HIGH) {
//
//				if (!measuringButtonTime && isMeasuring) {
//					timeButtonHigh = millis();
//					measuringButtonTime = true;
//				}
//
//				ledState = !ledState;
//				sendingEnabled = !sendingEnabled;
//				if (!sendingEnabled)
//				{
//					powercastControlLayer.resetMeasurement();
//				}
//
//				if (sendingEnabled)
//				{
//					rxMeasurementObject.saveButtonEventToFile(true);
//				}
//				else
//				{
//					rxMeasurementObject.saveButtonEventToFile(false);
//				}
//				
//
//				//startExperiment();
//			}
//			else if (buttonState == LOW) {
//				measuringButtonTime = false;
//			}
//
//			
//		}
//	}
//
//	// set the LED:
//	digitalWrite(ledPin, ledState);
//
//	// save the reading.  Next time through the loop,
//	// it'll be the lastButtonState:
//	lastButtonState = reading;
//
//}






