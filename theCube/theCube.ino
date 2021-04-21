#include <Arduino_LSM9DS1.h>


void setup() {
	Serial.begin(9600);
	IMU.begin();
}

void loop() {
	float x,y,z;

	if (IMU.gyroscopeAvailable()) {
		IMU.readGyroscope(x, y, z);

		Serial.print(int(x));
		Serial.print('\t');
		Serial.print(int(y));
		Serial.print('\t');
		Serial.print(int(z));
		Serial.println('\t');
	}
	delay(100);
}
