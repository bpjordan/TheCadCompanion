#include <ArduinoBLE.h>

#define DEBUG

BLEService sensorService("c9930826-a3ed-11eb-bcbc-0242ac130002");

BLEFloatCharacteristic gyroX("3366f988-a3ee-11eb-bcbc-0242ac130002", BLERead | BLEWrite);
BLEFloatCharacteristic gyroY("3367000e-a3ee-11eb-bcbc-0242ac130002", BLERead | BLEWrite);
BLEFloatCharacteristic gyroZ("336702e8-a3ee-11eb-bcbc-0242ac130002", BLERead | BLEWrite);

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

	//Add the service
	BLE.addService(sensorService);

	//Initialize all of the sensor values
	gyroX.writeValue(0);
	gyroY.writeValue(0);
	gyroZ.writeValue(0);

	//start advertising
	BLE.advertise();

	#ifdef DEBUG
	Serial.println("BLE Advertising");
	#endif

}

void loop() {
}
