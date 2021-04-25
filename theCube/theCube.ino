#include <ArduinoBLE.h>
#include <Arduino_LSM9DS1.h>

//#define DEBUG
#define BLEDEBUG

void setup() {

	Serial.begin(9600);

	#ifdef DEBUG
	while(!Serial);
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

	#ifdef BLEDEBUG
	Serial.println("BLE Initialized");
	#endif

	//Scan for the base station
	BLE.scanForUuid("c9930826-a3ed-11eb-bcbc-0242ac130002");

	#ifdef BLEDEBUG
	Serial.println("Scanning...");
	#endif

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

		//We found the right peripheral, so stop scanning
		BLE.stopScan();

		//And do the thing
		runSensors(baseStation);
		
		//When we get out of that function, we've lost BLE
		//So find it again
		BLE.scanForUuid("c9930826-a3ed-11eb-bcbc-0242ac130002");

		#ifdef BLEDEBUG
		Serial.println("Scanning...");
		#endif
	}
	
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


void runSensors(BLEDevice peripheral){
	//Connect to the base station
	#ifdef BLEDEBUG
	Serial.println("Connecting...");
	#endif

	if (!peripheral.connect()){
		#ifdef BLEDEBUG
		Serial.println("Connection failed!");
		#endif
		return;
	}
	
	#ifdef BLEDEBUG
	else {
		Serial.println("Connected");
	}
	#endif
	
	//Discover peripheral settings
	#ifdef BLEDEBUG
	Serial.println("Discovering attributes ...");
	#endif
	if (peripheral.discoverAttributes()) {
	#ifdef BLEDEBUG
		Serial.println("Attributes discovered");
	#endif
	} else {
	#ifdef BLEDEBUG
		Serial.println("Attribute discovery failed!");
	#endif
		peripheral.disconnect();
		return;
	}

	// Retrieve the characteristics we need
	BLECharacteristic gyroX = peripheral.characteristic("3366f988-a3ee-11eb-bcbc-0242ac130002");
	BLECharacteristic gyroY = peripheral.characteristic("3367000e-a3ee-11eb-bcbc-0242ac130002");
	BLECharacteristic gyroZ = peripheral.characteristic("336702e8-a3ee-11eb-bcbc-0242ac130002");

	if (!(gyroX && gyroY && gyroZ)){
		#ifdef BLEDEBUG
		Serial.println("Peripheral does not have one of the sensor characteristics");
		#endif
		return;
	} else if (!(gyroX.canWrite() && gyroY.canWrite() && gyroZ.canWrite())) {
		#ifdef BLEDEBUG
		Serial.println("One of the sensor characteristics is not writable");
		#endif
		return;
	}

	//Now for the good part
	while (peripheral.connected()) {
		//Do the sensor
		if (IMU.gyroscopeAvailable()) {
			float x, y, z;
			IMU.readGyroscope(x, y, z);

			#ifdef BLEDEBUG
			Serial.print(x, 0);
			Serial.print("   \t");
			Serial.print(y, 0);
			Serial.print("   \t");
			Serial.print(y, 0);
			Serial.print("   \r");
			#endif

			gyroX.writeValue(long(x));
			gyroY.writeValue(long(y));
			gyroZ.writeValue(long(z));
		}
	}
}
