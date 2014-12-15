#include <MemoryFree.h>

int x = 10;
int y = 11;
char z = 't';

void setup()
{

	Serial.begin(9600);

}

int loopNumber = 0;

void loop() {
	Serial.print("Loop ");
	Serial.print(++loopNumber);
	Serial.print(" freeMemory()= ");
	Serial.println(freeMemory());
	Serial.println(++loopNumber);
	
	byte* testArray;
	byte testArrayLength = 10;
	testArray = new byte[testArrayLength];

	Serial.print("TestArray: ");
	for (byte i = 0; i < testArrayLength; i++)
	{
		Serial.print(testArray[i], HEX);
		Serial.print(" - ");
	}

	delete[] testArray;

	delay(1000);

}
