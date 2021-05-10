#include <Arduino_LSM9DS1.h>


void setup()
{
    Serial.begin(38400);
    IMU.begin();
    digitalWrite(13, HIGH);
    while (!Serial);
    digitalWrite(13, LOW);
}

void loop()
{
    float x,y,z;

    if (IMU.gyroscopeAvailable()) {
        IMU.readGyroscope(x, y, z);

        Serial.flush();
        Serial.print(int(x-2.5));
        Serial.print(",");
        Serial.print(int(y));
        Serial.print(",");
        Serial.print(int(z));
    }
    Serial.println();
    delay(30);
}