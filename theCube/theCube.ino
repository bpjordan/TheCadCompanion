#include <ArduinoBLE.h>
#include <Arduino_LSM9DS1.h>

#define DEBUG

void setup() {

	#ifdef DEBUG
	Serial.begin(9600);
	#endif

	//Initialize Sensors
	IMU.begin();

	#ifdef DEBUG
	Serial.println("Sensors initialized");
	#endif



	#ifdef DEBUG
	Serial.println("X\tY\tZ");
	#endif
}

void loop() {
	float x,y,z;

	if (IMU.gyroscopeAvailable()) {
		IMU.readGyroscope(x, y, z);

		#ifdef DEBUG
		Serial.print(x, 0);
		Serial.print("   \t");
		Serial.print(y, 0);
		Serial.print("   \t");
		Serial.print(y, 0);
		Serial.print("   \r");
		#endif
	}
	delay(100);
}
