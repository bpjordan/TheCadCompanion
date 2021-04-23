#include <ArduinoBLE.h>
#include <Arduino_LSM9DS1.h>

//#define DEBUG
#define BLEDEBUG

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
	#else

	//Initialize BLE
	BLE.begin();

	//Scan for the base station
	BLE.scanForUuid("c9930826-a3ed-11eb-bcbc-0242ac130002")

	#endif
}

void loop(){

	//Skip over all of this if we're just debugging the sensor
	#ifdef DEBUG
	printSensors();
	#else

	BLEDevice baseStation = BLE.available();

	if (baseStation){
		#ifdef BLEDEBUG
		Serial.print("Found ");
		Serial.print(baseStation.address());
		Serial.print(" '");
		Serial.print(baseStation.localName());
		Serial.print(" '");
		Serial.print(baseStation.advertisedServiceUuid());
		Serial.println();
		#endif

		if (baseStation.localName() != "BaseStation")
			return;
		
	}

	//We found the right peripheral, so stop scanning
	BLE.stopScan();

	//And do the thing
	runSensors();
	
	//When we get out of that function, we've lost BLE
	//So find it again
	BLE.scanForUuid("c9930826-a3ed-11eb-bcbc-0242ac130002")

	#endif

}

#ifdef DEBUG
void printSensors() {
	float x,y,z;

	if (IMU.gyroscopeAvailable()) {
		IMU.readGyroscope(x, y, z);

		Serial.print(x, 0);
		Serial.print("   \t");
		Serial.print(y, 0);
		Serial.print("   \t");
		Serial.print(y, 0);
		Serial.print("   \r");
	}
	delay(100);
}
#endif


void runSensors(){
	
}
