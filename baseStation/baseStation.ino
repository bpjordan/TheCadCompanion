#include <ArduinoBLE.h>

#define DEBUG

BLEService sensorService("c9930826-a3ed-11eb-bcbc-0242ac130002");

BLELongCharacteristic gyroX("3366f988-a3ee-11eb-bcbc-0242ac130002", BLERead | BLEWrite);
BLELongCharacteristic gyroY("3367000e-a3ee-11eb-bcbc-0242ac130002", BLERead | BLEWrite);
BLELongCharacteristic gyroZ("336702e8-a3ee-11eb-bcbc-0242ac130002", BLERead | BLEWrite);

void setup() {

	Serial.begin(9600);
	while(!Serial);

	//Initialization sequence for BLE
	if (!BLE.begin()){
		Serial.println("BLE Initialization Failed");
		while(1);
	}
	#ifdef DEBUG
	else
		Serial.println("BLE Initialized");
	#endif

	//Set parameters to advertise
	BLE.setLocalName("BaseStation");
	BLE.setAdvertisedService(sensorService);

	//Add the characteristics
	sensorService.addCharacteristic(gyroX);
	sensorService.addCharacteristic(gyroY);
	sensorService.addCharacteristic(gyroZ);

	//Add the service
	BLE.addService(sensorService);



	//Initialize all of the sensor values
	gyroX.writeValue(0);
	gyroY.writeValue(0);
	gyroZ.writeValue(0);

	//start advertising
	BLE.advertise();

	#ifdef DEBUG
	Serial.println("BLE Advertising...");
	#endif

}

void loop() {
	//Listen to see if the cube is connected
	BLEDevice cube = BLE.central();

	//If the cube is connected:
	if (cube){
		readCube(cube);
		#ifdef DEBUG
		Serial.println("\rConnection Lost, waiting to reconnect");
		#endif
	}

}

void readCube(BLEDevice cube) {
	#ifdef DEBUG
	Serial.println("Cube Connected!");
	Serial.println("X\tY\tZ");
	Serial.println("====\t====\t====");
	#endif
	
	//While the cube is connected
	while(cube.connected()) {
		if (gyroX.written() || gyroY.written() || gyroZ.written()) {
			long x, y, z;
			gyroX.readValue(x);
			gyroY.readValue(y);
			gyroZ.readValue(z);

			#ifdef DEBUG
			Serial.print(x);
			Serial.print("   \t");
			Serial.print(y);
			Serial.print("   \t");
			Serial.print(z);
			Serial.print("   \t\r");
			#endif
		}
	}
}
